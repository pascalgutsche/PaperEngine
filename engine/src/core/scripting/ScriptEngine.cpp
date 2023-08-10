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

        std::unordered_map<UUID, std::unordered_map<Shr<ScriptClass>, EntityFieldStorage>> entityFieldStorage;

        //Runtime
    	Scene* sceneContext = nullptr;
    };

    static ScriptEngineData* script_data;


    

    void ScriptEngine::Init()
	{
        script_data = new ScriptEngineData();

        InitMono();

        LoadAssembly("resources/scripts/scriptcore.dll");
        LoadAppAssembly("SandboxProject/assets/scripts/bin/Sandbox.dll");

        ScriptGlue::RegisterComponents();
        ScriptGlue::RegisterFunctions();

        script_data->entityClass = MakeShr<ScriptClass>("Paper", "Entity", script_data->core_assembly_image);

        LoadAssemblyClasses(script_data->appAssembly);

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
        if (script_data->appAssembly)
            script_data->appAssemblyImage = mono_assembly_get_image(script_data->appAssembly);
        else
            LOG_CORE_ERROR("Could not find app assembly '{}'", filepath.string());
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

    void ScriptEngine::CreateScriptEntity(Entity entity)
    {
        LOG_CORE_DEBUG("CreateScriptEntity()");
        if (!entity.HasComponent<ScriptComponent>()) return;
        
        ScriptComponent& sc = entity.GetComponent<ScriptComponent>();
        
        if (!sc.scriptClass)
        {
            LOG_CORE_ERROR("tried to destroy script entity with entity '{}' because script was null", entity.GetUUID());
            return;
        }

        auto& entityFieldStorages = GetActiveEntityFieldStorageInternal(entity);

		
        const auto& scriptFields = sc.scriptClass->GetFields();
        for (auto& field : scriptFields)
        {
            bool found = false;
            for (auto& fieldStorage : entityFieldStorages)
            {
                if (fieldStorage->GetField() == field)
                    found = true;
            }

            if (!found)
                entityFieldStorages.push_back(MakeShr<ScriptFieldStorage>(&field));
        }
    }

    void ScriptEngine::DestroyScriptEntity(Entity entity)
    {
        LOG_CORE_DEBUG("DestroyScriptEntity()");
        if (!entity.HasComponent<ScriptComponent>()) return;
        
        ScriptComponent& sc = entity.GetComponent<ScriptComponent>();
        
        if (!sc.scriptClass)
        {
            LOG_CORE_ERROR("tried to destroy script entity with entity '{}' because script was null", entity.GetUUID());
            return;
        }
        script_data->entityFieldStorage.erase(entity.GetUUID());
    }

    void ScriptEngine::OnCreateEntity(Entity entity)
    {
        CORE_ASSERT(!script_data->entityInstances.contains(entity.GetUUID()), "")
        const auto& scrc = entity.GetComponent<ScriptComponent>();
        if (scrc.scriptClass)
        {
            Shr<EntityInstance> instance = MakeShr<EntityInstance>(scrc.scriptClass, entity);
            script_data->entityInstances[entity.GetUUID()] = instance;

            //apply class variables defined in editor
            if (script_data->entityFieldStorage.contains(entity.GetUUID()))
                for (const auto& fieldStorage : GetActiveEntityFieldStorage(entity))
					fieldStorage->SetRuntimeInstance(instance);

            instance->InvokeOnCreate();
        }
    }

    void ScriptEngine::OnDestroyEntity(Entity entity)
    {
        CORE_ASSERT(script_data->entityInstances.contains(entity.GetUUID()), "")

        auto it = script_data->entityInstances.find(entity.GetUUID());
        (*it).second->InvokeOnDestroy();

        //remove runtime instance of field storages
        if (script_data->entityFieldStorage.contains(entity.GetUUID()))
            for (const auto& fieldStorage : GetActiveEntityFieldStorage(entity))
                fieldStorage->RemoveRuntimeInstance();

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

    const EntityFieldStorage& ScriptEngine::GetActiveEntityFieldStorage(Entity entity)
    {
        ScriptComponent& sc = entity.GetComponent<ScriptComponent>();
        if (!script_data->entityFieldStorage.contains(entity.GetUUID()) || !script_data->entityFieldStorage.at(entity.GetUUID()).contains(sc.scriptClass))
        {
            LOG_CORE_ERROR("Does not have field storage for entity '{}'", entity.GetUUID().toString());
            return EntityFieldStorage();
        }
        return script_data->entityFieldStorage.at(entity.GetUUID()).at(sc.scriptClass);
    }

    std::unordered_map<Shr<ScriptClass>, EntityFieldStorage>& ScriptEngine::GetEntityFieldStorage(Entity entity)
    {
        return script_data->entityFieldStorage[entity.GetUUID()];
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

            std::string nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            std::string name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

            if (name == "<Module>") continue;

            Shr<ScriptClass> scriptClass = MakeShr<ScriptClass>(nameSpace, name);
            scriptClass->InitFieldMap();

            std::string fullName = scriptClass->GetFullClassName();
            if (scriptClass->IsSubclassOf(script_data->entityClass) && fullName != "Paper.Entity")
				script_data->entityClasses[fullName] = scriptClass;
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

    EntityFieldStorage& ScriptEngine::GetActiveEntityFieldStorageInternal(Entity entity)
    {
        ScriptComponent& sc = entity.GetComponent<ScriptComponent>();
        return script_data->entityFieldStorage[entity.GetUUID()][sc.scriptClass];
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
	    : classNameSpace(classNameSpace), className(className), fields(std::vector<ScriptField>())
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

    std::string ScriptClass::GetFullClassName() const
    {
        if (!classNameSpace.empty())
            return fmt::format("{}.{}", classNameSpace, className);
        return className;
    }

    MonoClass* ScriptClass::GetMonoClass() const
    {
        return monoClass;
    }

    ScriptField* ScriptClass::GetField(const std::string& fieldName)
    {
        ScriptField* scriptField = nullptr;
        for (ScriptField& field : fields)
            if (field.name == fieldName) scriptField = &field;
	    return scriptField;
    }

    bool ScriptClass::IsSubclassOf(const Shr<ScriptClass>& scriptClass) const
    {
        return mono_class_is_subclass_of(monoClass, scriptClass->monoClass, false);
    }

    void ScriptClass::InitFieldMap()
    {
	    const ScriptInstance tempInstance(shared_from_this());

        fields.reserve(mono_class_num_fields(monoClass));

        void* iterator = nullptr;
        while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
        {
            MonoType* monoType = mono_field_get_type(field);
            ScriptField& scriptField = fields.emplace_back();
            scriptField.name = mono_field_get_name(field);
			scriptField.type = Utils::MonoTypeToScriptFieldType(monoType);
            scriptField.flags = Utils::GetFieldFlags(mono_field_get_flags(field));
            int align;
            scriptField.typeSize = mono_type_size(monoType, &align);
            scriptField.monoField = field;
            scriptField.initialFieldVal = tempInstance.GetFieldValue(scriptField);
        }
    }


    //
    //  ScriptInstance
    //
    ScriptInstance::ScriptInstance(const Shr<ScriptClass>& scriptClass)
	    : scriptClass(scriptClass)
    {
        monoInstance = scriptClass->Instantiate();
    }

    Buffer& ScriptInstance::GetFieldValue(const ScriptField& scriptField) const
    {
        Buffer valBuffer;
        const auto& classFields = scriptClass->GetFields();
        if (std::find(classFields.begin(), classFields.end(), scriptField) == classFields.end()) return valBuffer;

        MonoObject* object = mono_field_get_value_object(mono_domain_get(), scriptField.monoField, monoInstance);

        valBuffer = Utils::MonoObjectToValue(scriptField.type, object);

        MonoClassField* lol = mono_class_get_field_from_name(scriptClass->GetMonoClass(), "UUID");

        //if (scriptField.type == ScriptFieldType::String)
        //{
        //    MonoString* string = (MonoString*)
        //    std::string valueStr = Utils::MonoStringToStdString(string);
        //    valBuffer.Allocate(valueStr.size() + 1);
        //    valBuffer.Nullify();
        //    valBuffer.Write(valueStr.data(), valueStr.size());
        //}
        //else
        //{
        //    valBuffer.Allocate(scriptField.typeSize);
        //    valBuffer.Nullify();
        //    mono_field_get_value(monoInstance, scriptField.monoField, valBuffer.data);
        //}
        return valBuffer;
    }

    void ScriptInstance::SetFieldValue(const ScriptField& scriptField, const Buffer& value) const
    {
        const auto& classFields = scriptClass->GetFields();
        if (std::find(classFields.begin(), classFields.end(), scriptField) == classFields.end()) return;

        void* data = nullptr;
        if (Utils::IsPrimitive(scriptField.type))
            data = value.data;
        else
            data = Utils::DataToMonoObject(scriptField.type, value.data);

        mono_field_set_value(monoInstance, scriptField.monoField, data);
    }

    //
    //  EntityInstance
    //
    EntityInstance::EntityInstance(const Shr<ScriptClass>& scriptClass, Entity entity)
	    : ScriptInstance(scriptClass)
    {
        CORE_ASSERT(scriptClass->IsSubclassOf(script_data->entityClass), "Class must be a subclass of entity");

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
        if (onCreateMethod)
			scriptClass->InvokeMethod(monoInstance, onCreateMethod);
    }

    void EntityInstance::InvokeOnDestroy() const
    {
        if (onDestroyMethod)
			scriptClass->InvokeMethod(monoInstance, onDestroyMethod);
    }

    void EntityInstance::InvokeOnUpdate(float dt) const
    {
        void* param = &dt;
        if (onUpdateMethod)
			scriptClass->InvokeMethod(monoInstance, onUpdateMethod, &param);
    }
}
