#include "Engine.h"
#include "ScriptEngine.h"

#include "ScriptGlue.h"
#include "ScriptAssembly.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/tabledefs.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"
#include <mono/metadata/attrdefs.h>

#include <filewatch/FileWatch.h>

#include "ScriptCache.h"
#include "component/ScriptComponent.h"
#include "generic/Application.h"
#include "generic/Entity.h"
#include "utils/Utils.h"


namespace Paper
{
    struct ScriptEngineData
    {
        MonoDomain* rootDomain = nullptr;
        MonoDomain* appDomain = nullptr;

        Shr<ScriptAssembly> coreAssembly;
		std::vector<Shr<ScriptAssembly>> appAssemblies;

        ManagedClass* entityClass = nullptr;

        std::unordered_map<PaperID, Shr<EntityInstance>> entityInstances;

        std::unordered_map<PaperID, std::unordered_map<CacheID, EntityFieldStorage>> entityFieldStorage;

        bool assembliesReloadSchedule = false;

        bool debugEnabled = true;

        bool monoInitialized = false;


        //Runtime
    	Scene* sceneContext = nullptr;
    };

    static ScriptEngineData* script_data;


    

    void ScriptEngine::Init()
	{
        script_data = new ScriptEngineData();

        ScriptCache::Init();
        InitMono();
        ScriptGlue::RegisterFunctions();

        script_data->appDomain = mono_domain_create_appdomain((char*)"PaperScriptRuntime", nullptr);
        mono_domain_set(script_data->appDomain, true);

        script_data->coreAssembly = MakeShr<ScriptAssembly>("resources/scripts/scriptcore.dll", true, true);
        ScriptGlue::RegisterComponents();

        std::filesystem::path sandbox = "SandboxProject/assets/scripts/bin/Sandbox.dll";
        if (std::filesystem::exists(sandbox))
        {
			script_data->appAssemblies.emplace_back(MakeShr<ScriptAssembly>(sandbox, true));
        }
        else
        {
            LOG_CORE_CRITICAL("could not find sandbox dll");
        }

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

	void ScriptEngine::Shutdown(bool appClose)
	{
        ScriptCache::Shutdown();
        ShutdownMono(appClose);
    	delete script_data;
	}

    //static void OnAppAssemblyFileEvent(const std::string& path, const filewatch::Event change_type)
    //{
    //    if (change_type == filewatch::Event::modified)
    //    {
    //        Application::SubmitToMainThread([]() {ScriptEngine::ReloadAppAssembly(true); });
    //    }
    //    LOG_CORE_DEBUG("FILEWATCH - Path: {} Event: {}", path, (int)change_type);
    //}

    void ScriptEngine::ReloadAppAssembly()
    {
        //if no assemblies to reload just return
        if (!script_data->assembliesReloadSchedule) return;
        script_data->assembliesReloadSchedule = false;
#if 0
        //cache scriptfieldstorage and restore it later
        std::unordered_map<Shr<EntityInstance>, std::unordered_map<Shr<ScriptField>, Buffer>> oldScriptFieldValues;
        for (const auto entityInstance : GetEntityInstances() | std::views::values)
        {
            for (auto& scriptField : entityInstance->GetScriptClass()->GetFields())
            {
                Buffer outBuffer;
                entityInstance->GetFieldValue(scriptField, outBuffer);
                oldScriptFieldValues[entityInstance][MakeShr<ScriptField>(scriptField)] = Buffer::Copy(outBuffer);
            }
        }

        SetToRootDomain();

        mono_domain_unload(script_data->appDomain);

        script_data->appDomain = mono_domain_create_appdomain((char*)"PaperScriptRuntime", nullptr);
        mono_domain_set(script_data->appDomain, true);

        script_data->coreAssembly->ReloadAssembly();

        ScriptGlue::RegisterComponents();

        for (const Shr<ScriptAssembly> scriptAssembly : script_data->appAssemblies)
            scriptAssembly->ReloadAssembly();



        for (const auto entityInstance : GetEntityInstances() | std::views::values)
        {
            Shr<ScriptClass> oldScriptClass = entityInstance->GetScriptClass();
            entityInstance->SetScriptClass(GetEntityInheritClass(oldScriptClass->GetFullClassName()));
            entityInstance->LoadMethods();
        }

        for (auto& [entityInstance, fieldValues] : oldScriptFieldValues)
        {
            for (auto [scriptField, buffer] : fieldValues)
            {
                for (const auto& newScriptField : entityInstance->GetScriptClass()->GetFields())
                {
	                if (scriptField->name == newScriptField.name && scriptField->stringType == newScriptField.stringType) 
                    {
                        entityInstance->SetFieldValueInternal(newScriptField, buffer.data);
                        break;
                    };
                }
                buffer.Release();
            }
        }
#endif
    }

    bool ScriptEngine::ShouldReloadAppAssembly()
    {
        return script_data->assembliesReloadSchedule;
    }

    void ScriptEngine::ScheduleAssemblyReload()
    {
        script_data->assembliesReloadSchedule = true;
    }

    void ScriptEngine::SetToRootDomain()
    {
        mono_domain_set(script_data->rootDomain, false);
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
        if (!entity.HasComponent<ScriptComponent>()) return;
        
        ScriptComponent& sc = entity.GetComponent<ScriptComponent>();
        
        if (!GetEntityInheritClass(sc.scriptClassName))
        {
            LOG_CORE_ERROR("tried to destroy script entity with entity '{}' because script was null", entity.GetPaperID());
            return;
        }

        auto& entityFieldStorages = GetActiveEntityFieldStorageInternal(entity);

		
        const auto& scriptFields = ScriptClass(GetEntityInheritClass(sc.scriptClassName)).GetFields();
        for (auto field : scriptFields)
        {
            bool found = false;
            for (auto& fieldStorage : entityFieldStorages)
            {
                if (fieldStorage->GetField() == field)
                    found = true;
            }

            if (!found)
                entityFieldStorages.push_back(MakeShr<ScriptFieldStorage>(field));
        }
    }

    void ScriptEngine::DestroyScriptEntity(Entity entity)
    {
        if (!entity.HasComponent<ScriptComponent>()) return;
        
        ScriptComponent& sc = entity.GetComponent<ScriptComponent>();
        
        if (!GetEntityInheritClass(sc.scriptClassName))
        {
            LOG_CORE_ERROR("tried to destroy script entity with entity '{}' because script was null", entity.GetPaperID());
            return;
        }
        script_data->entityFieldStorage.erase(entity.GetPaperID());
    }

    void ScriptEngine::OnCreateEntity(Entity entity)
    {
        CORE_ASSERT(!script_data->entityInstances.contains(entity.GetPaperID()), "")
        const auto& scrc = entity.GetComponent<ScriptComponent>();
        if (GetEntityInheritClass(scrc.scriptClassName))
        {
            Shr<EntityInstance> instance = MakeShr<EntityInstance>(GetEntityInheritClass(scrc.scriptClassName)->classID, entity);
            script_data->entityInstances[entity.GetPaperID()] = instance;

            //apply class variables defined in editor
            if (script_data->entityFieldStorage.contains(entity.GetPaperID()))
                for (const auto& fieldStorage : GetActiveEntityFieldStorage(entity))
                    fieldStorage->SetRuntimeInstance(instance);

            instance->InvokeOnCreate();
        }
        else
        {
            CORE_ASSERT(false, "");
            GetEntityInheritClass(scrc.scriptClassName);
        }
    }

    void ScriptEngine::OnDestroyEntity(Entity entity)
    {
        CORE_ASSERT(script_data->entityInstances.contains(entity.GetPaperID()), "")

        auto it = script_data->entityInstances.find(entity.GetPaperID());
        (*it).second->InvokeOnDestroy();

        //remove runtime instance of field storages
        if (script_data->entityFieldStorage.contains(entity.GetPaperID()))
            for (const auto& fieldStorage : GetActiveEntityFieldStorage(entity))
                fieldStorage->RemoveRuntimeInstance();

        script_data->entityInstances.erase(it);
    }

    void ScriptEngine::OnUpdateEntity(Entity entity, float dt)
    {
        CORE_ASSERT(script_data->entityInstances.contains(entity.GetPaperID()), "")

        script_data->entityInstances.at(entity.GetPaperID())->InvokeOnUpdate(dt);
    }

    bool ScriptEngine::EntityInheritClassExists(const std::string& fullClassName)
    {
	    const std::vector<ManagedClass*> entityInheritClasses = GetEntityInheritClasses();
        for (const ManagedClass* entityInheritClass : entityInheritClasses)
        {
            if (entityInheritClass->fullClassName == fullClassName)
                return true;
        }
        return false;
    }

    ManagedClass* ScriptEngine::GetEntityInheritClass(const std::string& fullClassName)
    {
        return ScriptCache::GetManagedClassFromName(fullClassName);
    }

    Scene* ScriptEngine::GetSceneContext()
    {
        return script_data->sceneContext;
    }

    Shr<EntityInstance> ScriptEngine::GetEntityScriptInstance(PaperID entityUUID)
    {
        if (!script_data->entityInstances.contains(entityUUID)) return nullptr;
        return script_data->entityInstances.at(entityUUID);
    }

    const EntityFieldStorage& ScriptEngine::GetActiveEntityFieldStorage(Entity entity)
    {
        ScriptComponent& sc = entity.GetComponent<ScriptComponent>();
        if (!script_data->entityFieldStorage.contains(entity.GetPaperID()) || !script_data->entityFieldStorage.at(entity.GetPaperID()).contains(GetEntityInheritClass(sc.scriptClassName)->classID))
        {
            LOG_CORE_ERROR("Does not have field storage for entity '{}'", entity.GetPaperID().toString());
            return EntityFieldStorage();
        }
        return script_data->entityFieldStorage.at(entity.GetPaperID()).at(GetEntityInheritClass(sc.scriptClassName)->classID);
    }

    std::unordered_map<CacheID, EntityFieldStorage>& ScriptEngine::GetEntityFieldStorage(Entity entity)
    {
        return script_data->entityFieldStorage[entity.GetPaperID()];
    }

    MonoDomain* ScriptEngine::GetDomain()
    {
        return script_data->appDomain;
    }

    ManagedClass* ScriptEngine::GetEntityClass()
    {
        return script_data->entityClass;
    }

    void ScriptEngine::SetEntityClass(ManagedClass* managedEntityClass)
    {
        script_data->entityClass = managedEntityClass;
    }

    std::vector<ManagedClass*> ScriptEngine::GetScriptClasses()
    {
        return ScriptCache::GetManagedClasses();
    }

    std::vector<ManagedClass*> ScriptEngine::GetEntityInheritClasses()
    {
        std::vector<ManagedClass*> managedClasses = ScriptCache::GetManagedClasses();
        std::vector<ManagedClass*> entityClasses;
        for (ManagedClass* managedClass : managedClasses)
        {
            if (managedClass->fullClassName == "Paper.Entity") continue;
            if (ScriptClass(managedClass).IsSubclassOf(script_data->entityClass))
                entityClasses.push_back(managedClass);
        }
        return entityClasses;
    }

    std::unordered_map<PaperID, Shr<EntityInstance>>& ScriptEngine::GetEntityInstances()
    {
        return script_data->entityInstances;
    }
    /*
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
    */
    void ScriptEngine::InitMono()
    {
        mono_set_assemblies_path("mono/lib");

        if (script_data->debugEnabled)
        {
            const char* argv[2] = {
                "--debugger-agent=transport=dt_socket,address=127.0.0.1:2550,server=y,suspend=n,loglevel=3,logfile=MonoDebugger.log",
                "--soft-breakpoints"
            };

            mono_jit_parse_options(2, (char**)argv);
            mono_debug_init(MONO_DEBUG_FORMAT_MONO);
        }

        MonoDomain* rootDomain = mono_jit_init("PaperJITRuntime");
        ASSERT(rootDomain, "");

        // Store the root domain pointer
        script_data->rootDomain = rootDomain;

        if (script_data->debugEnabled)
            mono_debug_domain_create(script_data->rootDomain);

        mono_thread_set_main(mono_thread_current());
    }

    void ScriptEngine::ShutdownMono(bool appClose)
    {
        SetToRootDomain();

        mono_domain_unload(script_data->appDomain);
        script_data->appDomain = nullptr;


        if (appClose)
        {
			mono_jit_cleanup(script_data->rootDomain);
			script_data->rootDomain = nullptr;
        }
    }

    EntityFieldStorage& ScriptEngine::GetActiveEntityFieldStorageInternal(Entity entity)
    {
        ScriptComponent& sc = entity.GetComponent<ScriptComponent>();
        return script_data->entityFieldStorage[entity.GetPaperID()][GetEntityInheritClass(sc.scriptClassName)->classID];
    }

    Shr<ScriptAssembly> ScriptEngine::GetCoreAssembly()
    {
        return script_data->coreAssembly;
    }

    std::vector<Shr<ScriptAssembly>>& ScriptEngine::GetAppAssemblies()
    {
        return script_data->appAssemblies;
    }


    //
    //  ScriptClass
    //
    ScriptClass::ScriptClass(CacheID classID)
        : id(classID), managedClass(ScriptCache::GetManagedClass(classID))
    {
    }

    ScriptClass::ScriptClass(ManagedClass* managedClass)
	    : id(managedClass->classID), managedClass(managedClass)
	{
    }

    ScriptClass::~ScriptClass()
    {
        id = 0;
        managedClass = nullptr;
    }

    MonoObject* ScriptClass::Instantiate() const
    {
        uint32_t classFlags = managedClass->classFlags; //if sealed, class has no constructor or is not constructable i.e enums and static classes
        if (classFlags & MONO_TYPE_ATTR_SEALED)
        {
            LOG_CORE_ERROR("Scripting: class is sealed and therefore it cannot be instatiated. '{}'", GetFullClassName());
            return nullptr;
        }
        if (!GetMethod(".ctor", 0))
        {
            LOG_CORE_ERROR("Scripting: class has no default constructor and therefore it cannot be default instatiated. '{}'", GetFullClassName());
            return nullptr;
        }

        MonoObject* monoObject = mono_object_new(script_data->appDomain, managedClass->monoClass);
        if (monoObject)
            mono_runtime_object_init(monoObject);
        else
            LOG_CORE_ERROR("Scripting: could not create object of class '{}'", GetFullClassName());
        return monoObject;
    }

    ManagedMethod* ScriptClass::GetMethod(const std::string& methodName, uint32_t paramCount) const
    {
        return ScriptCache::GetManagedMethod(managedClass, methodName, paramCount);
    }

    void ScriptClass::InvokeMethod(MonoObject* monoObject, const ManagedMethod* managedMethod, void** params) const
    {
        MonoObject* exc = nullptr;
        mono_runtime_invoke(managedMethod->monoMethod, monoObject, params, &exc);
        if (exc)
        {
            mono_print_unhandled_exception(exc);
        }
    }

    std::string ScriptClass::GetFullClassName() const
    {
        return managedClass->fullClassName;
    }

    MonoClass* ScriptClass::GetMonoClass() const
    {
        return managedClass->monoClass;
    }

    ManagedClass* ScriptClass::GetManagedClass() const
    {
        return managedClass;
    }

    std::vector<ManagedField*> ScriptClass::GetFields() const
    {
        std::vector<ManagedField*> classFields;
        for (const CacheID fieldID : managedClass->fieldIDs)
        {
            ManagedField* field = ScriptCache::GetManagedField(fieldID);
            classFields.push_back(field);
        }
        return classFields;
    }

    ManagedField* ScriptClass::GetField(const std::string& fieldName) const
    {
        for (const CacheID fieldID : managedClass->fieldIDs)
        {
            ManagedField* field = ScriptCache::GetManagedField(fieldID);
            if (field->fieldName == fieldName)
                return field;
        }
        return nullptr;
    }

    ScriptAssembly* ScriptClass::GetScriptAssembly() const
    {
        return managedClass->assembly;
    }

    bool ScriptClass::IsSubclassOf(const ScriptClass& scriptClass) const
    {
        return mono_class_is_subclass_of(managedClass->monoClass, scriptClass.GetMonoClass(), false);
    }

    //
    //  ScriptInstance
    //

    ScriptInstance::ScriptInstance(CacheID classID)
        : managedClassID(classID), managedClass(ScriptCache::GetManagedClass(classID))
    {
        monoInstance = ScriptClass(managedClassID).Instantiate();
    }

    ScriptInstance::ScriptInstance(ManagedClass* managedClass)
	    : managedClassID(managedClass->classID), managedClass(managedClass)
    {
        monoInstance = ScriptClass(managedClassID).Instantiate();
    }

    ScriptInstance::ScriptInstance(const ScriptClass& scriptClass)
	    : managedClassID(scriptClass.GetManagedClass()->classID), managedClass(scriptClass.GetManagedClass())
    {
        monoInstance = ScriptClass(managedClassID).Instantiate();
    }

    MonoObject* ScriptInstance::GetMonoInstance() const
    {
        return monoInstance; 
    }

    Buffer ScriptInstance::GetFieldValue(ManagedField* managedField) const
    {
        return ScriptUtils::GetFieldValue(monoInstance, managedField->fieldName, managedField->fieldType, managedField->isProperty);
    }

    void ScriptInstance::SetFieldValue(ManagedField* managedField, const void* value) const
    {
        ScriptUtils::SetFieldValue(monoInstance, managedField->fieldName, managedField->fieldType, managedField->isProperty, value);
    }

    ManagedClass* ScriptInstance::GetManagedClass() const
    {
        return managedClass;
    }

    /*
    ScriptInstance::ScriptInstance(MonoObject* instance)
        : monoInstance(instance) { }


    void ScriptInstance::SetFieldValueInternal(const ScriptField& scriptField, const void* value) const
    {
        const auto& classFields = scriptClass->GetFields();
        if (std::find(classFields.begin(), classFields.end(), scriptField) == classFields.end()) return;

        const void* data = nullptr;
        if (ScriptUtils::IsPrimitive(scriptField.type))
            data = value;
        else
            data = ScriptUtils::DataToMonoObject(scriptField.type, value);

        mono_field_set_value(monoInstance, scriptField.monoField, (void*)data);
    }
*/

    //
    //  EntityInstance
    //
    EntityInstance::EntityInstance(const CacheID entityScriptClassID, Entity entity)
        : ScriptInstance(entityScriptClassID), scriptClass(ScriptClass(entityScriptClassID))
    {
        CORE_ASSERT(ScriptClass(managedClassID).IsSubclassOf(script_data->entityClass), "Class must be a subclass of entity");

        LoadMethods();

        //constructor
        PaperID entityUUID = entity.GetPaperID();
        void* param = &entityUUID;
        scriptClass.InvokeMethod(monoInstance, constructor, &param);
    }

    void EntityInstance::LoadMethods()
    {
        constructor = ScriptClass(script_data->entityClass).GetMethod(".ctor", 1);
        onCreateMethod = scriptClass.GetMethod("OnCreate", 0);
        onDestroyMethod = scriptClass.GetMethod("OnDestroy", 0);
        onUpdateMethod = scriptClass.GetMethod("OnUpdate", 1);
    }

    void EntityInstance::InvokeOnCreate() const
    {
        if (onCreateMethod)
			scriptClass.InvokeMethod(monoInstance, onCreateMethod);
    }

    void EntityInstance::InvokeOnDestroy() const
    {
        if (onDestroyMethod)
			scriptClass.InvokeMethod(monoInstance, onDestroyMethod);
    }

    void EntityInstance::InvokeOnUpdate(float dt) const
    {
        void* param = &dt;
        if (onUpdateMethod)
			scriptClass.InvokeMethod(monoInstance, onUpdateMethod, &param);
    }
}
