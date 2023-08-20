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
    typedef struct _MonoProperty MonoProperty;
    typedef struct _MonoString MonoString;
}

namespace Paper
{
	class ScriptClass;
	struct ScriptField;
	using ScriptFieldFlags = uint32_t;
	enum class ScriptFieldFlag;
    enum class ScriptFieldType;


    class ScriptUtils
    {
    public:
        static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath, bool loadPDB = false);

        static void PrintAssemblyTypes(MonoAssembly* assembly);

        static ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType);

        static std::string ScriptFieldTypeToString(ScriptFieldType type);

        static uint32_t ScriptFieldTypeSize(ScriptFieldType type);

        static ScriptFieldFlags GetFieldFlags(uint32_t monoFieldFlags);

        static Buffer MonoObjectToValue(ScriptFieldType type, MonoObject* object);

        static std::string MonoStringToStdString(MonoString* monoString);

        static MonoString* StdStringToMonoString(const std::string& stdString);

        template<typename T>
        static T Unbox(MonoObject* obj) { return *(T*)UnboxInternal(obj); }

        template<typename T>
        static void WriteToBuffer(Buffer& writeBuffer, MonoObject* obj)
        {
            T value = Unbox<T>(obj);
            writeBuffer.Write(&value, sizeof(T));
        }

        static MonoObject* DataToMonoObject(ScriptFieldType type, const void* data);

        static bool IsPrimitive(ScriptFieldType type);

        static MonoObject* GetScriptFieldValueObject(const ScriptField& scriptField, MonoObject* monoInstance);

        static void CreateScriptField(ScriptField& scriptField, const std::string& name, MonoObject* reference, Shr<ScriptClass> classField = nullptr);

        static MonoObject* GetFieldValueObject(MonoObject* object, std::string_view fieldName, bool isProperty);

        static Buffer GetFieldValue(MonoObject* object, std::string_view fieldName, ScriptFieldType type, bool isProperty);

        static void SetFieldValue(MonoObject* object, ScriptFieldType type, const std::string& name, bool isProperty, const void* data, const Shr<ScriptClass>& baseClass = nullptr);
    private:
        static void* UnboxInternal(MonoObject* obj);

        friend class ScriptInstance;
    };
}