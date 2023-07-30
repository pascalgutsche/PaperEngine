#include "Engine.h"
#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include <glm/gtx/string_cast.hpp>


namespace Paper
{

    struct ScriptEngineData
    {
        MonoDomain* root_domain = nullptr;
        MonoDomain* app_domain = nullptr;

        MonoAssembly* core_assembly = nullptr;
    };

    static ScriptEngineData* data;

	void ScriptEngine::Init()
	{
        data = new ScriptEngineData();

        InitMono();
	}

	void ScriptEngine::Shutdown()
	{
        ShutdownMono();
        delete data;
	}

    char* ReadBytes(const std::string& filepath, uint32_t* outSize)
    {
        std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

        if (!stream)
        {
            // Failed to open the file
            return nullptr;
        }

        std::streampos end = stream.tellg();
        stream.seekg(0, std::ios::beg);
        uint32_t size = end - stream.tellg();

        if (size == 0)
        {
            // File is empty
            return nullptr;
        }

        char* buffer = new char[size];
        stream.read((char*)buffer, size);
        stream.close();

        *outSize = size;
        return buffer;
    }

    MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
    {
        uint32_t fileSize = 0;
        char* fileData = ReadBytes(assemblyPath, &fileSize);

        // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
        MonoImageOpenStatus status;
        MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

        if (status != MONO_IMAGE_OK)
        {
            const char* errorMessage = mono_image_strerror(status);
            // Log some error message using the errorMessage data
            return nullptr;
        }

        MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
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

            printf("%s.%s\n", nameSpace, name);
        }
    }

    static void NativeLog(MonoString* string, int level)
    {
        char* cstr = mono_string_to_utf8(string);
        std::string str(cstr);
        mono_free(cstr);

        switch (level)
        {
            case 0: LOG_SCR_DEBUG(str);
                break;
            case 1: LOG_SCR_TRACE(str);
                break;
            case 2: LOG_SCR_WARN(str);
                break;
            case 3: LOG_SCR_ERROR(str);
                break;
        }
    }
    
    static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
    {
        LOG_SCR_TRACE("Value: {0}", glm::to_string(*parameter));

        //*outResult = glm::cross(*parameter, glm::vec3(parameter->x, parameter->y, -parameter->z));
        *outResult = *parameter;
    }

    void ScriptEngine::InitMono()
    {
        mono_set_assemblies_path("mono/lib");


        MonoDomain* rootDomain = mono_jit_init("PaperJITRuntime");
        ASSERT(rootDomain, "");
        

        // Store the root domain pointer
        data->root_domain = rootDomain;

        // Create an App Domain
        data->app_domain = mono_domain_create_appdomain((char*)"PaperScriptRuntime", nullptr);
        mono_domain_set(data->app_domain, true);

        mono_add_internal_call("Paper.Main::NativeLog", NativeLog);
        mono_add_internal_call("Paper.Main::NativeLog_Vector", NativeLog_Vector);

        ///GO AWAY
        data->core_assembly = LoadCSharpAssembly("resources/scripts/scriptcore.dll");
        PrintAssemblyTypes(data->core_assembly);

        MonoImage* assembly_image = mono_assembly_get_image(data->core_assembly);
        MonoClass* mono_class = mono_class_from_name(assembly_image, "Paper", "Main");

        //
        MonoObject* instance = mono_object_new(data->app_domain, mono_class);
        mono_runtime_object_init(instance);

        //
        MonoMethod* print_method_func = mono_class_get_method_from_name(mono_class, "PrintMessage", 0);
        mono_runtime_invoke(print_method_func, instance, 0, 0);

        //
        MonoMethod* print_int_method_func = mono_class_get_method_from_name(mono_class, "PrintIntMessage", 1);

        int value = 88;
        void* param = &value;

        mono_runtime_invoke(print_int_method_func, instance, &param, nullptr);

        //
        MonoMethod* print_ints_method_func = mono_class_get_method_from_name(mono_class, "PrintIntsMessage", 2);

        int value1 = 18;
        int value2 = 420;
        void* params[2] = {
            &value1,
            &value2
        };

        mono_runtime_invoke(print_ints_method_func, instance, params, nullptr);

        //
        MonoMethod* print_custom_method_func = mono_class_get_method_from_name(mono_class, "PrintCustomMessage", 1);

        MonoString* mono_string = mono_string_new(data->app_domain, "Hello FROM C++ you bunker");

        void* string_param = mono_string;

        mono_runtime_invoke(print_custom_method_func, instance, &string_param, nullptr);

        ASSERT(false, "");
    }

    void ScriptEngine::ShutdownMono()
    {
        data->app_domain = nullptr;

        data->root_domain = nullptr;
    }

    
}
