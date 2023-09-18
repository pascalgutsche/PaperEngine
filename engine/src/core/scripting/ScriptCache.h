#pragma once
#include "ManagedTypes.h"
#include "scene/Entity.h"

namespace Paper
{
	using ClassCacheID = uint32_t;
	using FieldCacheID = uint32_t;
	using MethodCacheID = uint32_t;

	using FieldStorageCacheID = uint32_t;

	class ScriptCache
	{
	public:
		static void Init();
		static void Shutdown();

		static void ClearCache();
		static void ClearAssemblyCache(ScriptAssembly* assembly);

		static void CacheAssembly(ScriptAssembly* assembly);

		static FieldStorageCacheID CacheScriptFieldValue(FieldCacheID fieldID, Buffer value);

		static std::vector<ManagedClass*> GetManagedClasses();
		static ManagedClass* GetManagedClass(CacheID classID);
		static ManagedClass* GetManagedClassFromName(const std::string& fullClassName);

		static ManagedField* GetManagedField(CacheID fieldID);
		static ManagedMethod* GetManagedMethod(ManagedClass* managedClass, const std::string& name, uint32_t parameterCount = 0);

		static Buffer& GetFieldStorage(PaperID paperID, ManagedField* managedField);
	private:
		static CacheID CacheClass(const std::string& classNameSpace, const std::string& className, ScriptAssembly* assembly);
		static std::vector<CacheID> CacheFields(ManagedClass& managedClass, ScriptAssembly* assembly);
		static std::vector<CacheID> CacheMethods(ManagedClass& managedClass, ScriptAssembly* assembly);
	};
}
