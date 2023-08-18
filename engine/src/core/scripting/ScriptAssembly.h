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

		const std::unordered_map<std::string, Shr<ScriptClass>>& GetEntityInheritClasses() const { return entityClasses; };
		const std::unordered_map<std::string, Shr<ScriptClass>>& GetClasses() const { return classes; };

		static std::vector<ScriptAssembly*>& GetAllAssemblies() { return allAssemblies; }
	private:
		void LoadAssembly();
		void UnloadAssembly();

		void LoadAssemblyClasses();

		std::filesystem::path filePath = "";
		MonoAssembly* monoAssembly = nullptr;
		MonoImage* monoAssemblyImage = nullptr;

		std::unordered_map<std::string, Shr<ScriptClass>> classes;
		std::unordered_map<std::string, Shr<ScriptClass>> entityClasses;

		bool isCoreAssembly = false;
		bool containsPDB = false;

		static std::vector<ScriptAssembly*> allAssemblies;
	};
}
