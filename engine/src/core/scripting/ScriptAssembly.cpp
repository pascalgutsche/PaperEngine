#include "Engine.h"
#include "ScriptAssembly.h"
#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

#include <filewatch/FileWatch.h>


namespace Paper
{
	ScriptAssembly::ScriptAssembly(const std::filesystem::path& filePath, bool loadPDB, bool isCoreAssembly)
		: filePath(filePath), isCoreAssembly(isCoreAssembly), containsPDB(loadPDB)
	{
		assemblyFileWatcher = MakeScoped<filewatch::FileWatch<std::string>>(filePath.string(), [&](const std::string& path, const filewatch::Event change_type)
		{
			ScriptEngine::ScheduleAssemblyReload(this);
		});


	}

	void ScriptAssembly::ReloadAssembly()
	{
		UnloadAssembly();
		LoadAssembly();
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
			ScriptEngine::SetEntityClass(MakeShr<ScriptClass>("Paper", "Entity", monoAssemblyImage));

		entityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(monoAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			std::string nameSpace = mono_metadata_string_heap(monoAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			std::string name = mono_metadata_string_heap(monoAssemblyImage, cols[MONO_TYPEDEF_NAME]);

			if (name == "<Module>") continue;

			Shr<ScriptClass> scriptClass = MakeShr<ScriptClass>(nameSpace, name);
			scriptClass->InitFieldMap();

			std::string fullName = scriptClass->GetFullClassName();
			if (scriptClass->IsSubclassOf(ScriptEngine::GetEntityClass()) && fullName != "Paper.Entity")
				entityClasses[fullName] = scriptClass;
		}
	}
}
