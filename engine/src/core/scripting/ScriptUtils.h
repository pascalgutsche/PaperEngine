#pragma once
#include "generic/Buffer.h"

#include "ManagedTypes.h"

namespace Paper
{
    class ScriptUtils
    {
    public:
        static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath, bool loadPDB = false);
        static void PrintAssemblyTypes(MonoAssembly* assembly);

        static std::string MonoStringToStdString(MonoString* monoString);
        static std::string MonoCharPtrToStdString(char* monoCharPtr);
        static MonoString* StdStringToMonoString(const std::string& stdString);

        static bool IsPrimitive(ScriptFieldType type);
        static uint32_t ScriptFieldTypeSize(ScriptFieldType type);
        static std::string ScriptFieldTypeToString(ScriptFieldType type);
        static ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType);

        static ScriptFieldFlags GetFieldFlags(uint32_t monoFieldFlags);

        static Buffer MonoObjectToValue(ScriptFieldType type, MonoObject* object);
        static MonoObject* DataToMonoObject(ScriptFieldType type, const void* data);

        static MonoObject* GetFieldValueObject(MonoObject* object, std::string_view fieldName, bool isProperty);

        static Buffer GetFieldValue(MonoObject* object, std::string_view fieldName, ScriptFieldType type, bool isProperty);
        static void SetFieldValue(MonoObject* object, const std::string& fieldName, ScriptFieldType type, bool isProperty, const void* data, ManagedClass* baseClass = nullptr);
    };
}