#pragma once
#include "generic/Buffer.h"

extern "C"
{
    typedef struct _MonoClass MonoClass;
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoAssembly MonoAssembly;
    typedef struct _MonoMethod MonoMethod;
    typedef struct _MonoDomain MonoDomain;
    typedef struct _MonoImage MonoImage;
    typedef struct _MonoType MonoType;
    typedef struct _MonoClassField MonoClassField;
    typedef struct _MonoString MonoString;
}

namespace Paper
{
	struct ScriptField;
	using ScriptFieldFlags = uint32_t;
	enum class ScriptFieldFlag;
    enum class ScriptFieldType;


    class ScriptUtils
    {
    public:
        static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath);

        static void PrintAssemblyTypes(MonoAssembly* assembly);

        static ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType);

        static std::string ScriptFieldTypeToString(ScriptFieldType type);

        static uint32_t ScriptFieldTypeSize(ScriptFieldType type);

        static ScriptFieldFlags GetFieldFlags(uint32_t monoFieldFlags);

        static void MonoObjectToValue(const ScriptField& scriptField, MonoObject* object, Buffer& outBuffer);

        static std::string MonoStringToStdString(MonoString* monoString);

        static MonoString* StdStringToMonoString(const std::string& stdString);

        static MonoObject* DataToMonoObject(ScriptFieldType type, void* data);

        static bool IsPrimitive(ScriptFieldType type);

        static MonoObject* GetScriptFieldValueObject(const ScriptField& scriptField, MonoObject* monoInstance);

        static void CreateScriptField(ScriptField& scriptField, const std::string& name, MonoObject* reference);
    };
}