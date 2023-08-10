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
	using ScriptFieldFlags = uint32_t;
	enum class ScriptFieldFlag;
    enum class ScriptFieldType;


	namespace Utils
	{
        MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath);

        void PrintAssemblyTypes(MonoAssembly* assembly);

        ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType);

        std::string ScriptFieldTypeToString(ScriptFieldType type);

        uint32_t ScriptFieldTypeSize(ScriptFieldType type);

        ScriptFieldFlags GetFieldFlags(uint32_t monoFieldFlags);

        Buffer& MonoObjectToValue(ScriptFieldType type, MonoObject* object);

        std::string MonoStringToStdString(MonoString* monoString);

        MonoString* StdStringToMonoString(const std::string& stdString);

        MonoObject* DataToMonoObject(ScriptFieldType type, void* data);
        
        bool IsPrimitive(ScriptFieldType type);
	}
}