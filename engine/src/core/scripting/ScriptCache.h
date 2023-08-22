#pragma once
#include "ManagedTypes.h"
#include "ScriptAssembly.h"

namespace Paper
{
	class ScriptCache
	{
	public:
		static void Init();
		static void Shutdown();

		static void ClearCache();
		static void ClearAssemblyCache(ScriptAssembly* assembly);

		static void CacheAssembly(ScriptAssembly* assembly);

		static ManagedClass* GetManagedClass(CacheID classID);
		static ManagedField* GetManagedField(CacheID fieldID);
		static ManagedMethod* GetManagedMethod(CacheID methodID, uint32_t parameterCount = 0);

	private:
		static CacheID CacheClass(const std::string& classNameSpace, const std::string& className, ScriptAssembly* assembly);
		static std::vector<CacheID> CacheFields(ManagedClass& managedClass, ScriptAssembly* assembly);
		static std::vector<CacheID> CacheMethods(ManagedClass& managedClass, ScriptAssembly* assembly);
	};
}
