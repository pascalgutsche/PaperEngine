#include "Engine.h"
#include "ScriptCache.h"

#include "ManagedTypes.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include <mono/metadata/attrdefs.h>

#include "generic/Hash.h"

namespace Paper
{
	struct Cache
	{
		std::unordered_map<uint32_t, ManagedClass> managedClasses;
		std::unordered_map<uint32_t, ManagedField> managedFields;
		std::unordered_map<uint32_t, std::vector<ManagedMethod>> managedMethods;
	};

	static Cache* cache = nullptr;

	void ScriptCache::Init()
	{
		CORE_ASSERT(!cache, "Trying to initialize ScriptCache multiple times");
		cache = new Cache();
	}

	void ScriptCache::Shutdown()
	{
		ClearCache();
		delete cache;
		cache = nullptr;
	}

	void ScriptCache::ClearCache()
	{
		cache->managedClasses.clear();
		cache->managedFields.clear();
		cache->managedMethods.clear();
	}

	void ScriptCache::ClearAssemblyCache(ScriptAssembly* assembly)
	{
		for (auto& [id, managedClass] : cache->managedClasses)
		{
			if (managedClass.assembly != assembly) continue;
			cache->managedClasses.erase(id);
		}
		for (auto& [id, managedField] : cache->managedFields)
		{
			if (managedField.assembly != assembly) continue;
			cache->managedClasses.erase(id);
		}
		for (auto& [id, managedMethod] : cache->managedMethods)
		{
			if (managedMethod.begin()->assembly != assembly) continue;
			cache->managedClasses.erase(id);
		}
	}

	void ScriptCache::CacheAssembly(ScriptAssembly* assembly)
	{
		MonoImage* assemblyImage = assembly->GetMonoAssemblyImage();
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(assemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			std::string nameSpace = mono_metadata_string_heap(assemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			std::string name = mono_metadata_string_heap(assemblyImage, cols[MONO_TYPEDEF_NAME]);

			if (name == "<Module>") continue;

			std::string fullClassName = "";
			if (nameSpace.empty())
				fullClassName = name;
			else
				fullClassName = fmt::format("{}.{}", nameSpace, name);

			CacheID classID = CacheClass(fullClassName, assembly);

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

	CacheID ScriptCache::CacheClass(const std::string& fullName, ScriptAssembly* assembly)
	{
		CacheID id = Hash::GenerateFNVHash(fullName);

	}

	std::vector<CacheID> ScriptCache::CacheFields(CacheID classID)
	{
	}

	std::vector<CacheID> ScriptCache::CacheMethods(CacheID classID)
	{
	}
}
