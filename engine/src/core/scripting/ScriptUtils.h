#pragma once

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

        ScriptFieldFlags GetFieldFlags(uint32_t monoFieldFlags);
	}
}