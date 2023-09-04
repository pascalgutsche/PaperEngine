#include "Engine.h"
#include "ScriptAssembly.h"
#include "ScriptEngine.h"

#include "ScriptCache.h"

#include <filewatch/FileWatch.h>
#include <mono/metadata/assembly.h>

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
			ScriptEngine::ScheduleAssembliesReload();
		});

		LoadAssembly();

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
	}

	void ScriptAssembly::LoadAssembly()
	{
		monoAssembly = ScriptUtils::LoadMonoAssembly(filePath, containsPDB);

		if (monoAssembly)
			monoAssemblyImage = mono_assembly_get_image(monoAssembly);
		else
			LOG_CORE_ERROR("Could not find C# assembly '{}'", filePath.string());

		ScriptCache::CacheAssembly(this);
	}

	void ScriptAssembly::UnloadAssembly()
	{
		monoAssembly = nullptr;
		monoAssemblyImage = nullptr;
	}
}
