#include "Engine.h"
#include "ScriptCache.h"
#include "ManagedTypes.h"
#include "ScriptEngine.h"
#include "ScriptAssembly.h"

#include "generic/Hash.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/tabledefs.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/object.h>

namespace Paper
{
	struct Cache
	{
		std::unordered_map<uint32_t, ManagedClass> managedClasses;
		std::unordered_map<uint32_t, ManagedField> managedFields;
		std::unordered_map<uint32_t, std::vector<ManagedMethod>> managedMethods;
		std::vector<std::filesystem::path> assemblyPathsCached;
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
		cache->assemblyPathsCached.clear();
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
		if (std::ranges::find(cache->assemblyPathsCached, assembly->GetFilePath()) != cache->assemblyPathsCached.end())
		{
			LOG_CORE_WARN("Attempting to cache the same assembly multiple times.\n\tAssembly: '{}'", assembly->GetFilePath().string());
			return;
		}

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

			ManagedClass* managedClass = GetManagedClass(CacheClass(nameSpace, name, assembly));
			if (managedClass->fullClassName == "Paper.Entity")
				ScriptEngine::SetEntityClass(managedClass);
		}

		cache->assemblyPathsCached.push_back(assembly->GetFilePath());
	}

	std::vector<ManagedClass*> ScriptCache::GetManagedClasses()
	{
		std::vector<ManagedClass*> managedClasses;
		for (ManagedClass& managedClass : cache->managedClasses | std::views::values)
		{
			managedClasses.push_back(&managedClass);
		}
		return managedClasses;
	}

	ManagedClass* ScriptCache::GetManagedClass(CacheID classID)
	{
		if (!cache->managedClasses.contains(classID)) return nullptr;
		return &cache->managedClasses.at(classID);
	}

	ManagedClass* ScriptCache::GetManagedClassFromName(const std::string& fullClassName)
	{
		for (ManagedClass& managedClass : cache->managedClasses | std::views::values)
		{
			if (managedClass.fullClassName == fullClassName)
				return &managedClass;
		}
		return nullptr;
	}

	ManagedField* ScriptCache::GetManagedField(CacheID fieldID)
	{
		if (!cache->managedFields.contains(fieldID)) return nullptr;
		return &cache->managedFields.at(fieldID);
	}

	ManagedMethod* ScriptCache::GetManagedMethod(ManagedClass* managedClass, const std::string& name, uint32_t parameterCount)
	{
		std::string methodIDName = fmt::format("{}:{}", managedClass->fullClassName, name);
		std::string methodIDNameStd = std::format("{}:{}", managedClass->fullClassName, name);
		CacheID methodID = Hash::GenerateFNVHash(methodIDName);
		if (!cache->managedMethods.contains(methodID)) return nullptr;
		for (ManagedMethod& managedMethod : cache->managedMethods.at(methodID))
		{
			if (managedMethod.parameterCount == parameterCount)
				return &managedMethod;
		}
		return nullptr;
	}


	CacheID ScriptCache::CacheClass(const std::string& classNameSpace, const std::string& className, ScriptAssembly* assembly)
	{
		std::string fullClassName = "";
		if (classNameSpace.empty())
			fullClassName = className;
		else
			fullClassName = fmt::format("{}.{}", classNameSpace, className);

		CacheID classID = Hash::GenerateFNVHash(fullClassName);
		ManagedClass managedClass;
		managedClass.classID = classID;
		managedClass.classNameSpace = classNameSpace;
		managedClass.className = className;
		managedClass.fullClassName = fullClassName;
		managedClass.assembly = assembly;

		MonoImage* image = assembly->GetMonoAssemblyImage();
		MonoClass* monoClass = mono_class_from_name(image, classNameSpace.c_str(), className.c_str());
		CORE_ASSERT(monoClass, "");
		managedClass.monoClass = monoClass;
		managedClass.classFlags = mono_class_get_flags(monoClass);
		
		managedClass.methodIDs = CacheMethods(managedClass, assembly);
		managedClass.fieldIDs = CacheFields(managedClass, assembly);

		cache->managedClasses[classID] = managedClass;
		return classID;
	}

	std::vector<CacheID> ScriptCache::CacheFields(ManagedClass& managedClass, ScriptAssembly* assembly)
	{
		MonoObject* tempInstance = ScriptClass(&managedClass).Instantiate(false);
		std::vector<CacheID> fieldIDs;
		
		void* fieldIterator = nullptr;
		while (MonoClassField* field = mono_class_get_fields(managedClass.monoClass, &fieldIterator))
		{
			std::string fieldName = mono_field_get_name(field);
			if (fieldName.find(">k__BackingField") != std::string::npos) continue;
			std::string fieldIDName = fmt::format("{}_{}", managedClass.fullClassName, fieldName);
			CacheID fieldID = Hash::GenerateFNVHash(fieldIDName);
			ManagedField& managedField = cache->managedFields[fieldID];
			managedField.fieldID = fieldID;
			managedField.classID = managedClass.classID;
			managedField.fieldName = fieldName;
			managedField.isProperty = false;
			managedField.monoField = field;
			managedField.fieldFlags = mono_field_get_flags(field);
			managedField.accessibilityFlags = ScriptUtils::GetFieldFlags(managedField.fieldFlags);
			managedField.isStatic = managedField.fieldFlags & FIELD_ATTRIBUTE_STATIC;

			MonoType* monoType = mono_field_get_type(field);
			managedField.fieldType = ScriptUtils::MonoTypeToScriptFieldType(monoType);
			int align;
			managedField.monoFieldSize = mono_type_size(monoType, &align);

			if (tempInstance)
				managedField.initialFieldValue = ScriptUtils::GetFieldValue(tempInstance, managedField.fieldName, managedField.fieldType, managedField.isProperty);

			if (!managedField.initialFieldValue)
			{
				managedField.initialFieldValue.Allocate(managedField.monoFieldSize);
				managedField.initialFieldValue.Nullify();
			}

			managedField.assembly = assembly;

			fieldIDs.push_back(fieldID);
		}

		void* propertyIterator = nullptr;
		while (MonoProperty* property = mono_class_get_properties(managedClass.monoClass, &propertyIterator))
		{
			std::string propertyName = mono_property_get_name(property);
			std::string propertyIDName = fmt::format("{}_{}", managedClass.fullClassName, propertyName);
			CacheID propertyID = Hash::GenerateFNVHash(propertyIDName);
			ManagedField managedField;
			managedField.fieldID = propertyID;
			managedField.classID = managedClass.classID;
			managedField.fieldName = propertyName;
			managedField.isProperty = true;
			managedField.monoField = property;
			managedField.fieldFlags = mono_property_get_flags(property);
			managedField.accessibilityFlags = ScriptUtils::GetFieldFlags(managedField.fieldFlags);
			managedField.isStatic = managedField.fieldFlags & FIELD_ATTRIBUTE_STATIC;

			MonoMethod* getMethod = mono_property_get_get_method(property);
			if (getMethod)
			{
				MonoMethodSignature* getMethodSigniture = mono_method_get_signature(getMethod, nullptr, 0);
				MonoType* monoType = mono_signature_get_return_type(getMethodSigniture);
				managedField.fieldType = ScriptUtils::MonoTypeToScriptFieldType(monoType);

				int align;
				managedField.monoFieldSize = mono_type_size(monoType, &align);
			}

			if (tempInstance)
				managedField.initialFieldValue = ScriptUtils::GetFieldValue(tempInstance, managedField.fieldName, managedField.fieldType, managedField.isProperty);

			managedField.assembly = assembly;

			fieldIDs.push_back(propertyID);
			cache->managedFields[propertyID] = managedField;
		}

		return fieldIDs;
	}


	std::vector<CacheID> ScriptCache::CacheMethods(ManagedClass& managedClass, ScriptAssembly* assembly)
	{
		std::vector<CacheID> methodIDs;

		void* methodIterator = nullptr;
		while (MonoMethod* method = mono_class_get_methods(managedClass.monoClass, &methodIterator))
		{
			MonoMethodSignature* methodSig = mono_method_signature(method);
			
			std::string methodName = ScriptUtils::MonoCharPtrToStdString(mono_method_full_name(method, false));
			CacheID methodID = Hash::GenerateFNVHash(methodName);
			ManagedMethod managedMethod;
			managedMethod.methodID = methodID;
			managedMethod.classID = managedClass.classID;
			managedMethod.methodName = methodName;
			managedMethod.parameterCount = mono_signature_get_param_count(methodSig);
			managedMethod.monoMethod = method;
			managedMethod.methodFlags = mono_method_get_flags(method, nullptr);
			managedMethod.isStatic = managedMethod.methodFlags & METHOD_ATTRIBUTE_STATIC;
			managedMethod.isVirtual = managedMethod.methodFlags & METHOD_ATTRIBUTE_VIRTUAL;

			managedMethod.assembly = assembly;

			methodIDs.push_back(methodID);
			cache->managedMethods[methodID].push_back(managedMethod);
		}
		return methodIDs;
	}
}
