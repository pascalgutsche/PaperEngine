#include "Engine.h"
#include "ScriptUtils.h"

#include "ScriptEngine.h"
#include "utils/Utils.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/tabledefs.h>

namespace Paper
{
    std::unordered_map<std::string, ScriptFieldType> scriptFieldTypeMap {
        { "System.Char", ScriptFieldType::Not_Supported },
        { "System.SByte", ScriptFieldType::Not_Supported },
        { "System.Byte", ScriptFieldType::Not_Supported },
        { "System.Boolean", ScriptFieldType::Bool },
        { "System.Int16", ScriptFieldType::Short },
        { "System.UInt16", ScriptFieldType::UShort },
        { "System.Int32", ScriptFieldType::Int },
        { "System.UInt32", ScriptFieldType::UInt },
        { "System.Int64", ScriptFieldType::Long },
        { "System.UInt64", ScriptFieldType::ULong },
        { "System.Single", ScriptFieldType::Float },
        { "System.Double", ScriptFieldType::Double },
        { "System.String", ScriptFieldType::String },
        { "Paper.Vec2", ScriptFieldType::Vec2 },
        { "Paper.Vec3", ScriptFieldType::Vec3 },
        { "Paper.Vec4", ScriptFieldType::Vec4 },
        { "Paper.Entity", ScriptFieldType::Entity },

    };

    namespace Utils
    {
        MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath)
        {
            uint32_t fileSize = 0;
            char* fileData = ReadBytes(assemblyPath, &fileSize);

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

        void PrintAssemblyTypes(MonoAssembly* assembly)
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

        ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
        {
            char* typeName = mono_type_get_name(monoType);
            if (!scriptFieldTypeMap.contains(std::string(typeName))) return ScriptFieldType::Invalid;
            return scriptFieldTypeMap.at(std::string(typeName));
        }

        std::string ScriptFieldTypeToString(ScriptFieldType type)
        {
            switch (type)
            {
                case ScriptFieldType::None:     return "<None>";
                case ScriptFieldType::Invalid:  return "?";
                case ScriptFieldType::Not_Supported:  return "<WIP>";
                case ScriptFieldType::Bool:     return "Bool";
                case ScriptFieldType::Char:     return "Char";
                case ScriptFieldType::UChar:    return "Unsigned Char";
                case ScriptFieldType::Short:    return "Short";
                case ScriptFieldType::UShort:   return "Unsigned Short";
                case ScriptFieldType::Int:      return "Int";
                case ScriptFieldType::UInt:     return "Unsigned Int";
                case ScriptFieldType::Long:     return "Long";
                case ScriptFieldType::ULong:    return "Unsigned Long";
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

        ScriptFieldVisibilityBitMap GetFieldVisibility(uint32_t fieldFlags)
        {
            ScriptFieldVisibilityBitMap accessibility = (ScriptFieldVisibilityBitMap)ScriptFieldVisibility::None;
            uint32_t accessFlag = fieldFlags & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK;

            switch (accessFlag)
            {
                case FIELD_ATTRIBUTE_PRIVATE:
                {
                    accessibility = (ScriptFieldVisibilityBitMap)ScriptFieldVisibility::Private;
                    break;
                }
                case FIELD_ATTRIBUTE_FAM_AND_ASSEM:
                {
                    accessibility |= (ScriptFieldVisibilityBitMap)ScriptFieldVisibility::Protected;
                    accessibility |= (ScriptFieldVisibilityBitMap)ScriptFieldVisibility::Internal;
                    break;
                }
                case FIELD_ATTRIBUTE_ASSEMBLY:
                {
                    accessibility = (ScriptFieldVisibilityBitMap)ScriptFieldVisibility::Internal;
                    break;
                }
                case FIELD_ATTRIBUTE_FAMILY:
                {
                    accessibility = (ScriptFieldVisibilityBitMap)ScriptFieldVisibility::Protected;
                    break;
                }
                case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
                {
                    accessibility |= (ScriptFieldVisibilityBitMap)ScriptFieldVisibility::Private;
                    accessibility |= (ScriptFieldVisibilityBitMap)ScriptFieldVisibility::Protected;
                    break;
                }
                case FIELD_ATTRIBUTE_PUBLIC:
                {
                    accessibility = (ScriptFieldVisibilityBitMap)ScriptFieldVisibility::Public;
                    break;
                }
            }

            return accessibility;
        }
    }

    bool operator==(ScriptFieldVisibilityBitMap val, ScriptFieldVisibility visibility)
    {
        return val & (ScriptFieldVisibilityBitMap)visibility;
    }
}