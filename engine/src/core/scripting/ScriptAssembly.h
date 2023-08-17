#pragma once

#include "ScriptUtils.h"

namespace filewatch
{
	template<class StringType>
	class FileWatch;
}

namespace Paper
{
	class ScriptClass;

	class ScriptAssembly
	{
	public:
		ScriptAssembly() = default;
		ScriptAssembly(const std::filesystem::path& filePath, bool loadPDB = false, bool isCoreAssembly = false);

		void ReloadAssembly();

		bool IsCoreAssembly() const { return isCoreAssembly; };
	private:
		void LoadAssembly();
		void UnloadAssembly();

		void LoadAssemblyClasses();

		std::filesystem::path filePath = "";
		MonoAssembly* monoAssembly = nullptr;
		MonoImage* monoAssemblyImage = nullptr;

		Scope<filewatch::FileWatch<std::string>> assemblyFileWatcher;

		std::unordered_map<std::string, Shr<ScriptClass>> classes;
		std::unordered_map<std::string, Shr<ScriptClass>> entityClasses;

		bool isCoreAssembly = false;
		bool containsPDB = false;
	};
}
