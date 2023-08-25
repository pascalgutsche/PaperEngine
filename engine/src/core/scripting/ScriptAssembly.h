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

		ScriptAssembly(const ScriptAssembly&) = default;

		~ScriptAssembly();

		void ReloadAssembly();

		bool IsCoreAssembly() const { return isCoreAssembly; }
		MonoAssembly* GetMonoAssembly() const { return monoAssembly; }
		MonoImage* GetMonoAssemblyImage() const { return monoAssemblyImage; }
		std::string GetFileName() const { return filePath.filename().string(); }
		std::filesystem::path GetFilePath() const { return filePath; }

		static std::vector<ScriptAssembly*>& GetAllAssemblies() { return allAssemblies; }
	private:
		void LoadAssembly();
		void UnloadAssembly();

		std::filesystem::path filePath = "";
		MonoAssembly* monoAssembly = nullptr;
		MonoImage* monoAssemblyImage = nullptr;

		bool isCoreAssembly = false;
		bool containsPDB = false;

		static std::vector<ScriptAssembly*> allAssemblies;
	};
}
