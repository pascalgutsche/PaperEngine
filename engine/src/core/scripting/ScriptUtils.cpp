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

#include "utils/UUID.h"

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
        { "Paper.Entity", ScriptFieldType::Entity },

    };


    MonoAssembly* ScriptUtils::LoadMonoAssembly(const std::filesystem::path& assemblyPath)
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

        MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
        mono_image_close(image);

        // Don't forget to free the file data
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
            case ScriptFieldType::Entity:   return "Entity";
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
            case ScriptFieldType::Entity:   return sizeof(UUID);
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

    template<typename T>
    static T Unbox(MonoObject* obj) { return *(T*)mono_object_unbox(obj); }

    template<typename T>
    static void WriteToBuffer(Buffer& writeBuffer, MonoObject* obj)
    {
        T value = Unbox<T>(obj);
        writeBuffer.Write(&value, sizeof(T));
    }

    void ScriptUtils::MonoObjectToValue(const ScriptField& scriptField, MonoObject* object, Buffer& outBuffer)
    {
        if (!object) return;

        outBuffer.Allocate(ScriptFieldTypeSize(scriptField.type));
        outBuffer.Nullify();


        switch (scriptField.type)
        {
        case ScriptFieldType::None: 
        case ScriptFieldType::Invalid:
        case ScriptFieldType::Not_Supported:
            return;

        case ScriptFieldType::Bool:
            return WriteToBuffer<bool>(outBuffer, object);
        case ScriptFieldType::Char:
            return WriteToBuffer<int8_t>(outBuffer, object);
        case ScriptFieldType::UChar:
            return WriteToBuffer<uint8_t>(outBuffer, object);
        case ScriptFieldType::Int16:
            return WriteToBuffer<int16_t>(outBuffer, object);
        case ScriptFieldType::UInt16:
            return WriteToBuffer<uint16_t>(outBuffer, object);
        case ScriptFieldType::Int32:
            return WriteToBuffer<int32_t>(outBuffer, object);
        case ScriptFieldType::UInt32:
            return WriteToBuffer<uint32_t>(outBuffer, object);
        case ScriptFieldType::Int64:
            return WriteToBuffer<int64_t>(outBuffer, object);
        case ScriptFieldType::UInt64:
            return WriteToBuffer<uint64_t>(outBuffer, object);
        case ScriptFieldType::Float:
            return WriteToBuffer<float>(outBuffer, object);
        case ScriptFieldType::Double:
            return WriteToBuffer<double>(outBuffer, object);
        case ScriptFieldType::String:
        {
            std::string stdString = MonoStringToStdString((MonoString*)object);
            outBuffer.Allocate(stdString.size() + 1);
            outBuffer.Nullify();
            outBuffer.Write(stdString.data(), stdString.size());
            return;
        }
        case ScriptFieldType::Vec2:
            return WriteToBuffer<glm::vec2>(outBuffer, object);
        case ScriptFieldType::Vec3:
            return WriteToBuffer<glm::vec3>(outBuffer, object);
        case ScriptFieldType::Vec4:
            return WriteToBuffer<glm::vec4>(outBuffer, object);
        case ScriptFieldType::Entity:
        {
            ScriptField scriptFieldUUID;
            CreateScriptField(scriptFieldUUID, "UUID", object);

            Buffer uuidBuffer;
            MonoObjectToValue(scriptFieldUUID, GetScriptFieldValueObject(scriptFieldUUID, object), uuidBuffer);
            outBuffer.Write(uuidBuffer.data, sizeof(UUID));
            uuidBuffer.Release();
            return;
        }
        }
        CORE_ASSERT(false, "");
        return;
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

    MonoObject* ScriptUtils::DataToMonoObject(ScriptFieldType type, void* data)
    {
        switch (type)
        {
        case ScriptFieldType::String: 
            return (MonoObject*)StdStringToMonoString(std::string((char*)data));
        case ScriptFieldType::Entity:
            return ScriptClass("Paper", "Entity", ScriptEngine::GetCoreAssemblyImage()).InstantiateParams(*(UUID*)data);
        default:
            return nullptr;
        }
    }

    bool ScriptUtils::IsPrimitive(ScriptFieldType type)
    {
        switch (type) {
        case ScriptFieldType::String: 
        case ScriptFieldType::Vec2: 
        case ScriptFieldType::Vec3: 
        case ScriptFieldType::Vec4: 
        case ScriptFieldType::Entity: return false;
        default: return true;
        }
    }

    MonoObject* ScriptUtils::GetScriptFieldValueObject(const ScriptField& scriptField, MonoObject* monoInstance)
    {
        MonoClassField* classField = nullptr;
        if (scriptField.monoField)
            classField = scriptField.monoField;
        else
        {
            if (scriptField.name == "") return nullptr;
            MonoClass* objectClass = mono_object_get_class(monoInstance);
            classField = mono_class_get_field_from_name(objectClass, scriptField.name.data());
        }

        return mono_field_get_value_object(mono_domain_get(), classField, monoInstance);
    }

    void ScriptUtils::CreateScriptField(ScriptField& scriptField, const std::string& name, MonoObject* reference)
    {
        MonoClass* monoClass = mono_object_get_class(reference);
        MonoClassField* monoField = mono_class_get_field_from_name(monoClass, name.data());
        MonoType* monoType = mono_field_get_type(monoField);

        ScriptInstance scriptInstance(reference);

        scriptField.name = name;
        scriptField.type = MonoTypeToScriptFieldType(monoType);
        scriptField.flags = GetFieldFlags(mono_field_get_flags(monoField));
        int align;
        scriptField.typeSize = mono_type_size(monoType, &align);
        scriptField.monoField = monoField;
    	scriptInstance.GetFieldValue(scriptField, scriptField.initialFieldVal);

        // allocate memory to the buffer for later SetData when an entity field contains no entity.
        if (scriptField.type == ScriptFieldType::Entity)
        {
            scriptField.initialFieldVal.Allocate(sizeof(UUID));
            scriptField.initialFieldVal.Nullify();
        }
    }
}
