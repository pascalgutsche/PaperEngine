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

		static CacheID CacheClass(const std::string& fullName, ScriptAssembly* assembly);
		static CacheID CacheFields(CacheID classID);
		static CacheID CacheMethods(CacheID classID);
	};
}
