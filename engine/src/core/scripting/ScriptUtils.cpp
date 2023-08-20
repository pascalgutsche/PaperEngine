#include "Engine.h"
#include "ScriptUtils.h"

#include "ScriptEngine.h"
#include "ScriptField.h"
#include "utils/Utils.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/tabledefs.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/tokentype.h>
#include <mono/metadata/mono-debug.h>

#include "utils/PaperID.h"

namespace Paper
{
    std::unordered_map<std::string, ScriptFieldType> scriptFieldTypeMap {
        { "System.Char", ScriptFieldType::Char },
        { "System.SByte", ScriptFieldType::Not_Supported },
        { "System.Byte", ScriptFieldType::Not_Supported },
        { "System.Boolean", ScriptFieldType::Bool },
        { "System.Int16", ScriptFieldType::Int16 },
        { "System.UInt16", ScriptFieldType::UInt16 },
        { "System.Int32", ScriptFieldType::Int32 },
        { "System.UInt32", ScriptFieldType::UInt32 },
        { "System.Int64", ScriptFieldType::Int64 },
        { "System.UInt64", ScriptFieldType::UInt64 },
        { "System.Single", ScriptFieldType::Float },
        { "System.Double", ScriptFieldType::Double },
        { "System.String", ScriptFieldType::String },
        { "Paper.Vec2", ScriptFieldType::Vec2 },
        { "Paper.Vec3", ScriptFieldType::Vec3 },
        { "Paper.Vec4", ScriptFieldType::Vec4 },
        { "Paper.PaperID", ScriptFieldType::PaperID },
        { "Paper.Entity", ScriptFieldType::Entity },
        { "Paper.DataComponent", ScriptFieldType::DataComponent },
        { "Paper.TransformComponent", ScriptFieldType::TransformComponent },
		{ "Paper.SpriteComponent", ScriptFieldType::SpriteComponent},
        { "Paper.LineComponent", ScriptFieldType::LineComponent },
        { "Paper.TextComponent", ScriptFieldType::TextComponent },
        { "Paper.CameraComponent", ScriptFieldType::CameraComponent },
        { "Paper.ScriptComponent",  ScriptFieldType::ScriptComponent }

    };


    MonoAssembly* ScriptUtils::LoadMonoAssembly(const std::filesystem::path& assemblyPath, bool loadPDB)
    {
        uint32_t fileSize = 0;
        char* fileData = Utils::ReadBytes(assemblyPath, &fileSize);

        // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
        MonoImageOpenStatus status;
        MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

        if (status != MONO_IMAGE_OK)
        {
            const char* errorMessage = mono_image_strerror(status);
            LOG_CORE_ERROR("[SCRIPTCORE]: {}", errorMessage);
            return nullptr;
        }
        LOG_CORE_TRACE("Loaded assembly file: {}", assemblyPath);

        if (loadPDB)
        {
            std::filesystem::path pdbPath = assemblyPath;
            pdbPath.replace_extension(".pdb");

            if (std::filesystem::exists(pdbPath))
            {
                uint32_t pdbFileSize = 0;
                const char* pdbData = Utils::ReadBytes(pdbPath, &pdbFileSize);
                mono_debug_open_image_from_memory(image, (const mono_byte*)pdbData, pdbFileSize);
                LOG_CORE_TRACE("Loaded PDB file '{}' for assembly: '{}'", pdbPath, assemblyPath);
                delete[] pdbData;
            }
            else
                LOG_CORE_WARN("Could not find PDB file for assembly: '{}'", assemblyPath);
        }

        MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
        mono_image_close(image);

        delete[] fileData;

        return assembly;
    }

    void ScriptUtils::PrintAssemblyTypes(MonoAssembly* assembly)
    {
        MonoImage* image = mono_assembly_get_image(assembly);
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

        for (int32_t i = 0; i < numTypes; i++)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

            const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
            LOG_CORE_TRACE("{}.{}", nameSpace, name);
        }
    }

    ScriptFieldType ScriptUtils::MonoTypeToScriptFieldType(MonoType* monoType)
    {
        char* typeName = mono_type_get_name(monoType);
        if (!scriptFieldTypeMap.contains(std::string(typeName))) return ScriptFieldType::Invalid;
        return scriptFieldTypeMap.at(std::string(typeName));
    }

    std::string ScriptUtils::ScriptFieldTypeToString(ScriptFieldType type)
    {
        switch (type)
        {
            case ScriptFieldType::None:     return "<None>";
            case ScriptFieldType::Invalid:  return "?";
            case ScriptFieldType::Not_Supported:  return "<WIP>";
            case ScriptFieldType::Bool:     return "Bool";
            case ScriptFieldType::Char:     return "Char";
            case ScriptFieldType::UChar:    return "Unsigned Char";
            case ScriptFieldType::Int16:    return "Short";
            case ScriptFieldType::UInt16:   return "Unsigned Short";
            case ScriptFieldType::Int32:      return "Int";
            case ScriptFieldType::UInt32:     return "Unsigned Int";
            case ScriptFieldType::Int64:     return "Long";
            case ScriptFieldType::UInt64:    return "Unsigned Long";
            case ScriptFieldType::Float:    return "Float";
            case ScriptFieldType::Double:   return "Double";
            case ScriptFieldType::String:   return "String";
            case ScriptFieldType::Vec2:     return "Vec2";
            case ScriptFieldType::Vec3:     return "Vec3";
            case ScriptFieldType::Vec4:     return "Vec4";
            case ScriptFieldType::PaperID:   return "PaperID";
            case ScriptFieldType::Entity:   return "Entity";
            case ScriptFieldType::DataComponent:        return "DataComponent";
            case ScriptFieldType::TransformComponent:   return "TransformComponent";
			case ScriptFieldType::SpriteComponent:      return "SpriteComponent";
			case ScriptFieldType::LineComponent:        return "LineComponent";
			case ScriptFieldType::TextComponent:        return "TextComponent";
            case ScriptFieldType::CameraComponent:      return "CameraComponent";
            case ScriptFieldType::ScriptComponent:      return "ScriptComponent";
            default: return "<Invalid>";
        }
    }

    uint32_t ScriptUtils::ScriptFieldTypeSize(ScriptFieldType type)
    {
        switch (type)
        {
            case ScriptFieldType::Bool:     return sizeof(bool);
            case ScriptFieldType::Char:     return sizeof(char);
            case ScriptFieldType::UChar:    return sizeof(unsigned char);
            case ScriptFieldType::Int16:    return sizeof(int16_t);
            case ScriptFieldType::UInt16:   return sizeof(uint16_t);
			case ScriptFieldType::Int32:    return sizeof(int32_t);
            case ScriptFieldType::UInt32:   return sizeof(uint32_t);
            case ScriptFieldType::Int64:    return sizeof(int64_t);
            case ScriptFieldType::UInt64:   return sizeof(uint64_t);
            case ScriptFieldType::Float:    return sizeof(float);
            case ScriptFieldType::Double:   return sizeof(double);
            case ScriptFieldType::String:   return sizeof(char);
            case ScriptFieldType::Vec2:     return sizeof(glm::vec2);
            case ScriptFieldType::Vec3:     return sizeof(glm::vec3);
            case ScriptFieldType::Vec4:     return sizeof(glm::vec4);
            case ScriptFieldType::PaperID:
            case ScriptFieldType::Entity:
            case ScriptFieldType::DataComponent:
            case ScriptFieldType::TransformComponent:
            case ScriptFieldType::SpriteComponent:
            case ScriptFieldType::LineComponent:
            case ScriptFieldType::TextComponent:
            case ScriptFieldType::CameraComponent:
            case ScriptFieldType::ScriptComponent:
        		return sizeof(PaperID);
        }
        LOG_CORE_ERROR("No ScriptFieldType size for '{}'", ScriptFieldTypeToString(type));
        return 0;
    }

    ScriptFieldFlags ScriptUtils::GetFieldFlags(uint32_t monoFieldFlags)
    {
        ScriptFieldFlags fieldFlags = (ScriptFieldFlags)ScriptFieldFlag::None;
        uint32_t accessFlag = monoFieldFlags & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK;

        switch (accessFlag)
        {
            case FIELD_ATTRIBUTE_PRIVATE:
            {
                fieldFlags = (ScriptFieldFlags)ScriptFieldFlag::Private;
                break;
            }
            case FIELD_ATTRIBUTE_FAM_AND_ASSEM:
            {
                fieldFlags = (ScriptFieldFlags)ScriptFieldFlag::Protected;
                fieldFlags |= (ScriptFieldFlags)ScriptFieldFlag::Internal;
                break;
            }
            case FIELD_ATTRIBUTE_ASSEMBLY:
            {
                fieldFlags = (ScriptFieldFlags)ScriptFieldFlag::Internal;
                break;
            }
            case FIELD_ATTRIBUTE_FAMILY:
            {
                fieldFlags = (ScriptFieldFlags)ScriptFieldFlag::Protected;
                break;
            }
            case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
            {
                fieldFlags = (ScriptFieldFlags)ScriptFieldFlag::Private;
                fieldFlags |= (ScriptFieldFlags)ScriptFieldFlag::Protected;
                break;
            }
            case FIELD_ATTRIBUTE_PUBLIC:
            {
                fieldFlags = (ScriptFieldFlags)ScriptFieldFlag::Public;
                break;
            }
			case FIELD_ATTRIBUTE_INIT_ONLY:
            {
                fieldFlags |= (ScriptFieldFlags)ScriptFieldFlag::Readonly;
                break;
            }
        }

        return fieldFlags;
    }

    Buffer ScriptUtils::MonoObjectToValue(ScriptFieldType type, MonoObject* object)
    {
        Buffer outBuffer;
        if (!object) return outBuffer;

        outBuffer.Allocate(ScriptFieldTypeSize(type));
        outBuffer.Nullify();


        switch (type)
        {
        case ScriptFieldType::None: 
        case ScriptFieldType::Invalid:
        case ScriptFieldType::Not_Supported:
            break;

        case ScriptFieldType::Bool:
            WriteToBuffer<bool>(outBuffer, object);
            break;
        case ScriptFieldType::Char:
            WriteToBuffer<int8_t>(outBuffer, object);
            break;
        case ScriptFieldType::UChar:
            WriteToBuffer<uint8_t>(outBuffer, object);
            break;
        case ScriptFieldType::Int16:
            WriteToBuffer<int16_t>(outBuffer, object);
            break;
        case ScriptFieldType::UInt16:
            WriteToBuffer<uint16_t>(outBuffer, object);
            break;
        case ScriptFieldType::Int32:
            WriteToBuffer<int32_t>(outBuffer, object);
            break;
        case ScriptFieldType::UInt32:
            WriteToBuffer<uint32_t>(outBuffer, object);
            break;
        case ScriptFieldType::Int64:
            WriteToBuffer<int64_t>(outBuffer, object);
            break;
        case ScriptFieldType::UInt64:
            WriteToBuffer<uint64_t>(outBuffer, object);
            break;
        case ScriptFieldType::Float:
            WriteToBuffer<float>(outBuffer, object);
            break;
        case ScriptFieldType::Double:
            WriteToBuffer<double>(outBuffer, object);
            break;
        case ScriptFieldType::String:
        {
            std::string stdString = MonoStringToStdString((MonoString*)object);
            outBuffer.Allocate(stdString.size() + 1);
            outBuffer.Nullify();
            outBuffer.Write(stdString.data(), stdString.size());
            break;
        }
        case ScriptFieldType::Vec2:
            WriteToBuffer<glm::vec2>(outBuffer, object);
            break;
        case ScriptFieldType::Vec3:
            WriteToBuffer<glm::vec3>(outBuffer, object);
            break;
        case ScriptFieldType::Vec4:
            WriteToBuffer<glm::vec4>(outBuffer, object);
            break;
        case ScriptFieldType::PaperID:
        {
            Buffer idBuffer = GetFieldValue(object, "_id", ScriptFieldType::UInt64, false);
            outBuffer.Write(idBuffer.data, sizeof(uint64_t));
            idBuffer.Release();
            break;
        }

        case ScriptFieldType::Entity:
        {
            Buffer paperIDBuffer = GetFieldValue(object, "PaperID", ScriptFieldType::PaperID, false);
            outBuffer.Write(paperIDBuffer.data, sizeof(PaperID));
            paperIDBuffer.Release();
            break;
        }

        case ScriptFieldType::DataComponent:
        case ScriptFieldType::TransformComponent:
        case ScriptFieldType::SpriteComponent:
        case ScriptFieldType::LineComponent:
        case ScriptFieldType::TextComponent:
        case ScriptFieldType::CameraComponent:
        case ScriptFieldType::ScriptComponent:
        {
            Buffer entityBuffer = GetFieldValue(object, "Entity", ScriptFieldType::Entity, true);
            outBuffer.Write(entityBuffer.data, sizeof(PaperID));
            entityBuffer.Release();
            break;
        }
        }
        return outBuffer;
    }

    std::string ScriptUtils::MonoStringToStdString(MonoString* monoString)
    {
        if (!monoString) return "";

        char* text = mono_string_to_utf8(monoString);
        std::string value = std::string(text);
        mono_free(text);
        return value;
    }

    MonoString* ScriptUtils::StdStringToMonoString(const std::string& stdString)
    {
        return mono_string_new(mono_domain_get(), stdString.c_str());
    }

    MonoObject* ScriptUtils::DataToMonoObject(ScriptFieldType type, const void* data)
    {
        switch (type)
        {
        case ScriptFieldType::String: 
            return (MonoObject*)StdStringToMonoString(std::string((const char*)data));
        case ScriptFieldType::PaperID:
            return ScriptClass("Paper", "PaperID", *ScriptEngine::GetCoreAssembly().get()).InstantiateParams(*(uint64_t*)data);
        case ScriptFieldType::Entity:
            return ScriptClass("Paper", "Entity", *ScriptEngine::GetCoreAssembly().get()).InstantiateParams(*(PaperID*)data);
        case ScriptFieldType::DataComponent:
        case ScriptFieldType::TransformComponent:
        case ScriptFieldType::SpriteComponent:
        case ScriptFieldType::LineComponent:
        case ScriptFieldType::TextComponent:
        case ScriptFieldType::CameraComponent:
        case ScriptFieldType::ScriptComponent:
	        {
				MonoObject* object = ScriptClass("Paper", ScriptFieldTypeToString(type), *ScriptEngine::GetCoreAssembly().get()).Instantiate();
                
                SetFieldValue(object, ScriptFieldType::Entity, "Entity", true, data, MakeShr<ScriptClass>("Paper", "Component", *ScriptEngine::GetCoreAssembly().get()));
                return object;
	        }

        	default:
            return nullptr;
        }
    }

    bool ScriptUtils::IsPrimitive(ScriptFieldType type)
    {
        switch (type) {
            case ScriptFieldType::Bool:
            case ScriptFieldType::Char:
            case ScriptFieldType::UChar:
            case ScriptFieldType::Int16:
            case ScriptFieldType::UInt16:
            case ScriptFieldType::Int32:
            case ScriptFieldType::UInt32:
            case ScriptFieldType::Int64:
            case ScriptFieldType::UInt64:
            case ScriptFieldType::Float:
            case ScriptFieldType::Double:
        	return true;
        default: return false;
        }
    }

    MonoObject* ScriptUtils::GetScriptFieldValueObject(const ScriptField& scriptField, MonoObject* monoInstance)
    {
        
        if (scriptField.isProperty)
			return mono_property_get_value(scriptField.monoProperty, monoInstance, nullptr, nullptr);
        return mono_field_get_value_object(mono_domain_get(), scriptField.monoField, monoInstance);
    }

    void ScriptUtils::CreateScriptField(ScriptField& scriptField, const std::string& name, MonoObject* reference, Shr<ScriptClass> fieldClass)
    {
        MonoClass* monoClass = nullptr; 
        if (fieldClass)
            monoClass = fieldClass->GetMonoClass();
        else
            monoClass = mono_object_get_class(reference);
        MonoType* monoType = nullptr; 
        MonoClassField* monoField = mono_class_get_field_from_name(monoClass, name.data());
        MonoProperty* monoProperty = mono_class_get_property_from_name(monoClass, name.data());

        if (monoProperty)
        {
            MonoMethod* getMethod = mono_property_get_get_method(monoProperty);
            MonoMethodSignature * getMethodSigniture = mono_method_get_signature(getMethod, nullptr, 0);
            monoType = mono_signature_get_return_type(getMethodSigniture);
        }
        else
        {
            monoType = mono_field_get_type(monoField);
        }

        ScriptInstance scriptInstance(reference);

        scriptField.name = name;
        scriptField.type = MonoTypeToScriptFieldType(monoType);
        scriptField.stringType = mono_type_get_name(monoType);
        ASSERT(scriptField.stringType != "", "");
        int align;
        scriptField.typeSize = mono_type_size(monoType, &align);
        scriptField.monoField = monoField;
        scriptField.monoProperty = monoProperty;
        if (monoProperty) 
        {
            scriptField.flags = GetFieldFlags(mono_property_get_flags(monoProperty));
            scriptField.isProperty = true;
        }
        else
        {
            scriptField.flags = GetFieldFlags(mono_field_get_flags(monoField));
        }
    	scriptInstance.GetFieldValue(scriptField, scriptField.initialFieldVal);

        // allocate memory to the buffer for later SetData when an entity field contains no entity.
        if (scriptField.type == ScriptFieldType::Entity)
        {
            scriptField.initialFieldVal.Allocate(sizeof(PaperID));
            scriptField.initialFieldVal.Nullify();
        }
    }

    void* ScriptUtils::UnboxInternal(MonoObject* obj)
    {
        if (!obj) return nullptr;
        return mono_object_unbox(obj);
    }

    Buffer ScriptUtils::GetFieldValue(MonoObject* object, std::string_view fieldName, ScriptFieldType type, bool isProperty)
    {
        return MonoObjectToValue(type, GetFieldValueObject(object, fieldName, isProperty));
    }

    MonoObject* ScriptUtils::GetFieldValueObject(MonoObject* object, std::string_view fieldName, bool isProperty)
    {

        MonoClass* objectClass = mono_object_get_class(object);

        MonoObject* valueObject = nullptr;

        if (isProperty)
        {
            MonoProperty* classProperty = mono_class_get_property_from_name(objectClass, fieldName.data());
            valueObject = mono_property_get_value(classProperty, object, nullptr, nullptr);
        }
        else
        {
            MonoClassField* classField = mono_class_get_field_from_name(objectClass, fieldName.data());
            valueObject = mono_field_get_value_object(mono_domain_get(), classField, object);
        }

        return valueObject;
    }

    void ScriptUtils::SetFieldValue(MonoObject* object, ScriptFieldType type, const std::string& name, bool isProperty, const void* data, const Shr<ScriptClass>& baseClass)
    {
        if (object == nullptr || data == nullptr)
            return;

        MonoClass* objectClass = nullptr;
        if (baseClass)
            objectClass = baseClass->GetMonoClass();
        else
            objectClass = mono_object_get_class(object);

        if (isProperty)
        {
            MonoProperty* classProperty = mono_class_get_property_from_name(objectClass, name.c_str());
            void* propertyData = nullptr;

            if (IsPrimitive(type))
                propertyData = (void*)data;
            else
                propertyData = DataToMonoObject(type, data);

            mono_property_set_value(classProperty, object, &propertyData, nullptr);
        }
        else
        {
            MonoClassField* classField = mono_class_get_field_from_name(objectClass, name.c_str());
            void* fieldData = nullptr;

            if (IsPrimitive(type))
                fieldData = (void*)data;
            else
                fieldData = DataToMonoObject(type, data);


            mono_field_set_value(object, classField, fieldData);
        }
    }
}
