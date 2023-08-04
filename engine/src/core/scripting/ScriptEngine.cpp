#include "Engine.h"
#include "ScriptEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/tabledefs.h>

#include "ScriptGlue.h"
#include "component/ScriptComponent.h"
#include "generic/Application.h"
#include "generic/Entity.h"
#include "utils/Utils.h"


namespace Paper
{
    struct ScriptEngineData
    {
        MonoDomain* root_domain = nullptr;
        MonoDomain* app_domain = nullptr;

        MonoAssembly* core_assembly = nullptr;
        MonoImage* core_assembly_image = nullptr;

    	MonoAssembly* appAssembly = nullptr;
        MonoImage*  appAssemblyImage = nullptr;

        Shr<ScriptClass> entityClass = nullptr;

        std::unordered_map<std::string, Shr<ScriptClass>> entityClasses;
        std::unordered_map<UUID, Shr<EntityInstance>> entityInstances;

        //Runtime
    	Scene* sceneContext = nullptr;
    };

    static ScriptEngineData* script_data;


    bool EntityInstance::GetFieldValueVoid(const std::string& fieldName, void* buffer)
    {
        const auto& fields = scriptClass->GetFields();

		for (const auto& field : fields)
		{
            if (field.name != fieldName) continue;

            mono_field_get_value(monoInstance, field.monoField, buffer);
            return true;
		}

        return false;
    }

    void EntityInstance::SetFieldValueVoid(const std::string& fieldName, void* val)
    {
        const auto& fields = scriptClass->GetFields();

        for (const auto& field : fields)
        {
            if (field.name != fieldName) continue;

            mono_field_set_value(monoInstance, field.monoField, val);
        }

    }

    void ScriptEngine::Init()
	{
        script_data = new ScriptEngineData();

        InitMono();

        LoadAssembly("resources/scripts/scriptcore.dll");
        LoadAppAssembly("SandboxProject/assets/scripts/bin/Sandbox.dll");
        script_data->entityClass = MakeShr<ScriptClass>("Paper", "Entity", script_data->core_assembly_image);

        LoadAssemblyClasses(script_data->appAssembly);

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

    void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
    {
        script_data->appAssembly = Utils::LoadMonoAssembly(filepath);
        script_data->appAssemblyImage = mono_assembly_get_image(script_data->appAssembly);
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

    Shr<ScriptClass> ScriptEngine::GetEntityClass(const std::string& fullClassName)
    {
        if (!script_data->entityClasses.contains(fullClassName)) return nullptr;
        return script_data->entityClasses.at(fullClassName);
    }

    Scene* ScriptEngine::GetSceneContext()
    {
        return script_data->sceneContext;
    }

    Shr<EntityInstance> ScriptEngine::GetEntityScriptInstance(UUID entityUUID)
    {
        if (!script_data->entityInstances.contains(entityUUID)) return nullptr;
        return script_data->entityInstances.at(entityUUID);
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

            MonoClass* monoClass = mono_class_from_name(script_data->appAssemblyImage, nameSpace, name);
            bool isEntity = mono_class_is_subclass_of(monoClass, script_data->entityClass->monoClass, false);

            Shr<ScriptClass> scriptClass = MakeShr<ScriptClass>(nameSpace, name);

            if (isEntity && fullName != "Paper.Entity")
				script_data->entityClasses[fullName] = scriptClass;


            void* iterator = nullptr;
            while (MonoClassField* field = mono_class_get_fields(scriptClass->monoClass, &iterator))
            {
                std::string fieldName = mono_field_get_name(field);
                ScriptFieldType fieldType = Utils::MonoTypeToScriptFieldType(mono_field_get_type(field));
                ScriptFieldVisibilityBitMap fieldVisibility = Utils::GetFieldVisibility(mono_field_get_flags(field));
                scriptClass->fields.emplace_back<ScriptField>({ fieldName, fieldType, fieldVisibility, field });
            }
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

    MonoImage* ScriptEngine::GetAppAssemblyImage()
    {
        return script_data->appAssemblyImage;
    }

    MonoImage* ScriptEngine::GetCoreAssemblyImage()
    {
        return script_data->core_assembly_image;
    }

    //
    //  ScriptClass
    //
    ScriptClass::ScriptClass(const std::string& classNameSpace, const std::string& className, MonoImage* monoImage)
	    : classNameSpace(classNameSpace), className(className)
    {
        if (!monoImage) monoImage = script_data->appAssemblyImage;
        monoClass = mono_class_from_name(monoImage, classNameSpace.c_str(), className.c_str());
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
