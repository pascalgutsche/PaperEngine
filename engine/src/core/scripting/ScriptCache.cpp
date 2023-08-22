#include "Engine.h"
#include "ScriptCache.h"

#include "ManagedTypes.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/tabledefs.h>

#include "mono/metadata/object.h"

#include "ScriptEngine.h"
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

	ManagedClass* ScriptCache::GetManagedClass(CacheID classID)
	{
		if (!cache->managedClasses.contains(classID)) return nullptr;
		return &cache->managedClasses.at(classID);
	}

	ManagedField* ScriptCache::GetManagedField(CacheID fieldID)
	{
		if (!cache->managedFields.contains(fieldID)) return nullptr;
		return &cache->managedFields.at(fieldID);
	}

	ManagedMethod* ScriptCache::GetManagedMethod(CacheID methodID, uint32_t parameterCount)
	{
		if (!cache->managedMethods.contains(methodID)) return nullptr;
		for (ManagedMethod& managedMethod : cache->managedClasses.at(methodID))
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

		CacheID id = Hash::GenerateFNVHash(fullClassName);
		ManagedClass managedClass;
		managedClass.classID = id;
		managedClass.classNameSpace = classNameSpace;
		managedClass.className = className;
		managedClass.fullClassName = fullClassName;
		managedClass.assembly = assembly;

		MonoImage* image = assembly->GetMonoAssemblyImage();
		MonoClass* monoClass = mono_class_from_name(image, classNameSpace.c_str(), className.c_str());
		CORE_ASSERT(monoClass, "");
		managedClass.monoClass = monoClass;
		managedClass.classFlags = mono_class_get_flags(monoClass);
		
		managedClass.fieldIDs = CacheFields(managedClass, assembly);
		managedClass.methodIDs = CacheMethods(managedClass, assembly);


		cache->managedClasses[id] = managedClass;
	}

	std::vector<CacheID> ScriptCache::CacheFields(ManagedClass& managedClass, ScriptAssembly* assembly)
	{
		MonoObject* tempInstance = ScriptClass(managedClass.classID).Instantiate();
		std::vector<CacheID> fieldIDs;
		
		void* fieldIterator = nullptr;
		while (MonoClassField* field = mono_class_get_fields(managedClass.monoClass, &fieldIterator))
		{
			std::string fieldName = mono_field_get_name(field);
			std::string fieldIDName = fmt::format("{}_{}", managedClass.fullClassName, fieldName);
			CacheID fieldID = Hash::GenerateFNVHash(fieldIDName);
			ManagedField managedField;
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

			managedField.initialFieldValue = ScriptUtils::GetFieldValue(tempInstance, managedField.fieldName, managedField.fieldType, managedField.isProperty);

			managedField.assembly = assembly;

			fieldIDs.push_back(fieldID);
			cache->managedFields[fieldID] = managedField;
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
			MonoMethodSignature* getMethodSigniture = mono_method_get_signature(getMethod, nullptr, 0);
			MonoType* monoType = mono_signature_get_return_type(getMethodSigniture);

			managedField.fieldType = ScriptUtils::MonoTypeToScriptFieldType(monoType);
			int align;
			managedField.monoFieldSize = mono_type_size(monoType, &align);

			managedField.initialFieldValue = ScriptUtils::GetFieldValue(tempInstance, managedField.fieldName, managedField.fieldType, managedField.isProperty);

			managedField.assembly = assembly;

			fieldIDs.push_back(propertyID);
			cache->managedFields[propertyID] = managedField;
		}

		return 
	}

	std::vector<CacheID> ScriptCache::CacheMethods(ManagedClass& managedClass, ScriptAssembly* assembly)
	{
	}
}
