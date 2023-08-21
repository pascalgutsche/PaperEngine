#include "Engine.h"
#include "ScriptAssembly.h"
#include "ScriptEngine.h"

#include <filewatch/FileWatch.h>


namespace Paper
{
	std::vector<ScriptAssembly*> ScriptAssembly::allAssemblies;

	static std::unordered_map<ScriptAssembly*, Scope<filewatch::FileWatch<std::string>>> assemblyFileWatchers;

	ScriptAssembly::ScriptAssembly(const std::filesystem::path& filePath, bool loadPDB, bool isCoreAssembly)
		: filePath(filePath), isCoreAssembly(isCoreAssembly), containsPDB(loadPDB)
	{
		assemblyFileWatchers[this] = MakeScoped<filewatch::FileWatch<std::string>>(filePath.string(), [&](const std::string& path, const filewatch::Event change_type)
		{
				if (change_type != filewatch::Event::modified) return;
			LOG_CORE_WARN("FILE CHANGED: {} {}", path, (int)change_type);
			ScriptEngine::ScheduleAssemblyReload();
		});

		LoadAssembly();
		LoadAssemblyClasses();

		allAssemblies.push_back(this);
	}

	ScriptAssembly::~ScriptAssembly()
	{
		const auto it = std::find(allAssemblies.begin(), allAssemblies.end(), this);
		if (it != allAssemblies.end())
			allAssemblies.erase(it);

		assemblyFileWatchers.erase(this);
	}

	void ScriptAssembly::ReloadAssembly()
	{
		UnloadAssembly();
		LoadAssembly();
		LoadAssemblyClasses();
	}

	void ScriptAssembly::LoadAssembly()
	{
		monoAssembly = ScriptUtils::LoadMonoAssembly(filePath, containsPDB);

		if (monoAssembly)
			monoAssemblyImage = mono_assembly_get_image(monoAssembly);
		else
			LOG_CORE_ERROR("Could not find C# assembly '{}'", filePath.string());
	}

	void ScriptAssembly::UnloadAssembly()
	{
		monoAssembly = nullptr;
		monoAssemblyImage = nullptr;

		classes.clear();
		entityClasses.clear();
	}

	void ScriptAssembly::LoadAssemblyClasses()
	{
		if (isCoreAssembly)
			ScriptEngine::SetEntityClass(MakeShr<ScriptClass>("Paper", "Entity", *this));

		entityClasses.clear();
		classes.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(monoAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			std::string nameSpace = mono_metadata_string_heap(monoAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			std::string name = mono_metadata_string_heap(monoAssemblyImage, cols[MONO_TYPEDEF_NAME]);

			if (name == "<Module>") continue;

			Shr<ScriptClass> scriptClass = MakeShr<ScriptClass>(nameSpace, name, *this);

			uint32_t classFlags = mono_class_get_flags(scriptClass->monoClass); //if sealed, class has no constructor or is not constructable i.e enums and static classes
			if (classFlags & MONO_TYPE_ATTR_SEALED) continue;

			scriptClass->InitFieldMap();

			std::string fullName = scriptClass->GetFullClassName();
			if (scriptClass->IsSubclassOf(ScriptEngine::GetEntityClass()) && fullName != "Paper.Entity")
				entityClasses[fullName] = scriptClass;
			classes[fullName] = scriptClass;
		}
	}
}
