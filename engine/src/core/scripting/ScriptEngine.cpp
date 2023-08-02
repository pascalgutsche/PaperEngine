#include "Engine.h"
#include "ScriptEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include "ScriptGlue.h"
#include "component/ScriptComponent.h"
#include "generic/Application.h"
#include "generic/Entity.h"
#include "utils/Utils.h"


namespace Paper
{

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

    }

    struct ScriptEngineData
    {
        MonoDomain* root_domain = nullptr;
        MonoDomain* app_domain = nullptr;

        MonoAssembly* core_assembly = nullptr;
        MonoImage* core_assembly_image = nullptr;

        Shr<ScriptClass> entityClass = nullptr;

        std::unordered_map<std::string, Shr<ScriptClass>> entityClasses;
        std::unordered_map<UUID, Shr<EntityInstance>> entityInstances;

        //Runtime
    	Scene* sceneContext = nullptr;
    };

    static ScriptEngineData* script_data;

    

	void ScriptEngine::Init()
	{
        script_data = new ScriptEngineData();

        InitMono();

        LoadAssembly("resources/scripts/scriptcore.dll");
        script_data->entityClass = MakeShr<ScriptClass>("Paper", "Entity");

        LoadAssemblyClasses(script_data->core_assembly);

        ScriptGlue::RegisterComponents();
        ScriptGlue::RegisterFunctions();


#if 0
		ScriptClass mainClass("Paper", "Main");
        MonoObject* main_instance = mainClass.Instantiate();

        //
        mainClass.CallMethod(main_instance, "PrintMessage", 0);

        //
        int value = 88;
        void* param = &value;
        mainClass.CallMethod(main_instance, "PrintIntMessage", 1, &param);

        //
        int value1 = 18;
        int value2 = 420;
        void* params[2] = {
            &value1,
            &value2
        };
        mainClass.CallMethod(main_instance, "PrintIntsMessage", 2, params);

        //
        MonoString* mono_string = mono_string_new(script_data->app_domain, "Hello FROM C++ you bunker");
        void* string_param = mono_string;
        mainClass.CallMethod(main_instance, "PrintCustomMessage", 1, &string_param);
#endif
	}

	void ScriptEngine::Shutdown()
	{
        ShutdownMono();
        delete script_data;
	}

    void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
    {
        script_data->app_domain = mono_domain_create_appdomain((char*)"PaperScriptRuntime", nullptr);
        mono_domain_set(script_data->app_domain, true);

        script_data->core_assembly = Utils::LoadMonoAssembly(filepath);
        script_data->core_assembly_image = mono_assembly_get_image(script_data->core_assembly);
    }

    void ScriptEngine::OnRuntimeStart(Scene* scene)
    {
        script_data->sceneContext = scene;
    }

    void ScriptEngine::OnRuntimeStop()
    {
        script_data->sceneContext = nullptr;

        script_data->entityInstances.clear();
    }

    void ScriptEngine::OnCreateEntity(Entity entity)
    {
        CORE_ASSERT(!script_data->entityInstances.contains(entity.GetUUID()), "")
        const auto& scrc = entity.GetComponent<ScriptComponent>();
        if (EntityClassExists(scrc.name))
        {
            Shr<EntityInstance> instance = MakeShr<EntityInstance>(script_data->entityClasses[scrc.name], entity);
            script_data->entityInstances[entity.GetUUID()] = instance;

            instance->InvokeOnCreate();
        }
    }

    void ScriptEngine::OnDestroyEntity(Entity entity)
    {
        CORE_ASSERT(script_data->entityInstances.contains(entity.GetUUID()), "")

        auto it = script_data->entityInstances.find(entity.GetUUID());
        (*it).second->InvokeOnDestroy();
        script_data->entityInstances.erase(it);
    }

    void ScriptEngine::OnUpdateEntity(Entity entity, float dt)
    {
        CORE_ASSERT(script_data->entityInstances.contains(entity.GetUUID()), "")

        script_data->entityInstances.at(entity.GetUUID())->InvokeOnUpdate(dt);
    }

    bool ScriptEngine::EntityClassExists(const std::string& fullClassName)
    {
        return script_data->entityClasses.contains(fullClassName);
    }

    Scene* ScriptEngine::GetSceneContext()
    {
        return script_data->sceneContext;
    }

    MonoDomain* ScriptEngine::GetDomain()
    {
        return script_data->app_domain;
    }

    std::unordered_map<std::string, Shr<ScriptClass>>& ScriptEngine::GetEntityClasses()
    {
        return script_data->entityClasses;
    }

    std::unordered_map<UUID, Shr<EntityInstance>>& ScriptEngine::GetEntityInstances()
    {
        return script_data->entityInstances;
    }

    void ScriptEngine::LoadAssemblyClasses(MonoAssembly* monoAssembly)
    {
        script_data->entityClasses.clear();

        MonoImage* image = mono_assembly_get_image(monoAssembly);
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

        MonoClass* entityClass = mono_class_from_name(script_data->core_assembly_image, "Paper", "Entity");

		for (int32_t i = 0; i < numTypes; i++)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

            const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

            std::string fullName;
            if (strlen(nameSpace) != 0)
                fullName = fmt::format("{}.{}", nameSpace, name);
            else
                fullName = name;

            MonoClass* monoClass = mono_class_from_name(script_data->core_assembly_image, nameSpace, name);
            bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);

            if (isEntity && fullName != "Paper.Entity")
            {
                script_data->entityClasses[fullName] = MakeShr<ScriptClass>(nameSpace, name);
            }

            LOG_CORE_TRACE("{}.{} | isEntity {}", nameSpace, name, isEntity);
        }
    }

    void ScriptEngine::InitMono()
    {
        mono_set_assemblies_path("mono/lib");

        MonoDomain* rootDomain = mono_jit_init("PaperJITRuntime");
        ASSERT(rootDomain, "");

        // Store the root domain pointer
        script_data->root_domain = rootDomain;
    }

    void ScriptEngine::ShutdownMono()
    {
        script_data->app_domain = nullptr;

        script_data->root_domain = nullptr;
    }

    MonoImage* ScriptEngine::GetCoreAssemblyImage()
    {
        return script_data->core_assembly_image;
    }

    //
    //  ScriptClass
    //
    ScriptClass::ScriptClass(const std::string& classNameSpace, const std::string& className)
	    : classNameSpace(classNameSpace), className(className)
    {
        monoClass = mono_class_from_name(script_data->core_assembly_image, classNameSpace.c_str(), className.c_str());
    }

    MonoObject* ScriptClass::Instantiate() const
    {
        MonoObject* monoObject = mono_object_new(script_data->app_domain, monoClass);
        mono_runtime_object_init(monoObject);
        return monoObject;
    }

    MonoMethod* ScriptClass::GetMethod(const std::string& methodName, uint32_t paramCount) const
    {
        return mono_class_get_method_from_name(monoClass, methodName.c_str(), paramCount);
    }

    void ScriptClass::InvokeMethod(MonoObject* monoObject, MonoMethod* monoMethod, void** params) const
    {
        mono_runtime_invoke(monoMethod, monoObject, params, nullptr);
    }

    bool ScriptClass::IsSubclassOf(const Shr<ScriptClass>& scriptClass) const
    {
        return mono_class_is_subclass_of(monoClass, scriptClass->monoClass, false);
    }

    //
    //  ScriptInstance
    //
    EntityInstance::EntityInstance(const Shr<ScriptClass>& scriptClass, Entity entity)
	    : scriptClass(scriptClass)
    {
        CORE_ASSERT(scriptClass->IsSubclassOf(script_data->entityClass), "Class must be a subclass of entity");

        monoInstance = scriptClass->Instantiate();

        constructor = script_data->entityClass->GetMethod(".ctor", 1);
        onCreateMethod = scriptClass->GetMethod("OnCreate", 0);
        onDestroyMethod = scriptClass->GetMethod("OnDestroy", 0);
        onUpdateMethod = scriptClass->GetMethod("OnUpdate", 1);

        //constructor
        UUID entityUUID = entity.GetUUID();
        void* param = &entityUUID;
        scriptClass->InvokeMethod(monoInstance, constructor, &param);
    }

    void EntityInstance::InvokeOnCreate() const
    {
        scriptClass->InvokeMethod(monoInstance, onCreateMethod);
    }

    void EntityInstance::InvokeOnDestroy() const
    {
        scriptClass->InvokeMethod(monoInstance, onDestroyMethod);
    }

    void EntityInstance::InvokeOnUpdate(float dt) const
    {
        void* param = &dt;
        scriptClass->InvokeMethod(monoInstance, onUpdateMethod, &param);
    }
}
