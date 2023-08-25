#pragma once
#include "generic/Buffer.h"

#include "ManagedTypes.h"

namespace Paper
{
	class ScriptClass;
	
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
        static std::string MonoCharPtrToStdString(char* monoCharPtr);

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

        static MonoObject* GetFieldValueObject(MonoObject* object, std::string_view fieldName, bool isProperty);

        static Buffer GetFieldValue(MonoObject* object, std::string_view fieldName, ScriptFieldType type, bool isProperty);
        static void SetFieldValue(MonoObject* object, const std::string& fieldName, ScriptFieldType type, bool isProperty, const void* data, ManagedClass* baseClass = nullptr);

    private:
        static void* UnboxInternal(MonoObject* obj);

        friend class ScriptInstance;
    };
}