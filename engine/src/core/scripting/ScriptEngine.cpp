#include "Engine.h"
#include "ScriptEngine.h"

#include "ScriptGlue.h"
#include "ScriptAssembly.h"
#include "ScriptCache.h"
#include "ScriptFieldStorage.h"

#include "component/ScriptComponent.h"
#include "generic/Application.h"
#include "scene/Entity.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/attrdefs.h>

#include <filewatch/FileWatch.h>




namespace Paper
{
    struct ScriptEngineData
    {
        MonoDomain* rootDomain = nullptr;
        MonoDomain* appDomain = nullptr;

        ScriptAssembly coreAssembly;
		std::vector<ScriptAssembly> appAssemblies;

        ManagedClass* entityClass = nullptr;

        std::unordered_map<PaperID, EntityInstance> entityInstances;

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

        script_data->coreAssembly = ScriptAssembly("resources/scripts/scriptcore.dll", true, true);
        ScriptGlue::RegisterComponents();
	}

	void ScriptEngine::Shutdown(bool appClose)
	{
        ScriptCache::Shutdown();
        ShutdownMono(appClose);
    	delete script_data;
	}

	void ScriptEngine::ResetEngine()
	{
        script_data->entityInstances.clear();
        script_data->entityFieldStorage.clear();
        script_data->appAssemblies.clear();
        script_data->sceneContext = nullptr;

        ReloadAssemblies(true);
	}

	void ScriptEngine::AddAppAssembly(const std::filesystem::path& assemblyPath)
	{
        if (!std::filesystem::exists(assemblyPath))
        {
            LOG_CORE_ERROR("Could not find app assembly '{}'", assemblyPath);
            return;
        }
        script_data->appAssemblies.emplace_back(assemblyPath, true);
    	ReloadAssemblies(true);
	}

	void ScriptEngine::RemoveAppAssembly(const std::filesystem::path& assemblyPath)
	{
        for (int i = 0; i < script_data->appAssemblies.size(); i++)
        {
	        if (script_data->appAssemblies[i].GetFilePath() == assemblyPath)
	        {
                script_data->appAssemblies.erase(script_data->appAssemblies.begin() + i);
	        }
        }
    	ReloadAssemblies(true);
	}

	void ScriptEngine::ClearAppAssemblies()
	{
        script_data->appAssemblies.clear();
        ReloadAssemblies(true);
	}

	void ScriptEngine::ReloadAssemblies(bool bypassReloadScheduleCheck)
    {
        if (!bypassReloadScheduleCheck)
        {
            //if no assemblies to reload just return
            if (!script_data->assembliesReloadSchedule) return;
            script_data->assembliesReloadSchedule = false;

            LOG_CORE_TRACE("Assembly change detected");
            LOG_CORE_TRACE("\t->Reloading script assemblies");
        }

        //cache scriptfieldstorage and restore it later
        std::unordered_map<PaperID, std::unordered_map<CacheID, Buffer>> oldScriptFieldValues;
        for (const auto& [entityID, entityInstance] : GetEntityInstances())
        {
            for (auto fieldID : entityInstance->GetManagedClass()->fieldIDs)
            {
                ManagedField* managedField = ScriptCache::GetManagedField(fieldID);
                oldScriptFieldValues[entityID][fieldID] = entityInstance->GetFieldValue(managedField);;
            }
            DestroyScriptEntity(script_data->sceneContext->GetEntity(entityID));
        }
        script_data->entityInstances.clear();

        SetToRootDomain();

        mono_domain_unload(script_data->appDomain);

        script_data->appDomain = mono_domain_create_appdomain((char*)"PaperScriptRuntime", nullptr);
        mono_domain_set(script_data->appDomain, true);

        ScriptCache::ClearCache();

        script_data->coreAssembly.ReloadAssembly();

        ScriptGlue::RegisterComponents();

        for (ScriptAssembly& scriptAssembly : script_data->appAssemblies)
            scriptAssembly.ReloadAssembly();


        for (const auto& [entityID, classFieldMapBuffer] : oldScriptFieldValues)
        {
            Entity entity = script_data->sceneContext->GetEntity(entityID);
            const auto& scrc = entity.GetComponent<ScriptComponent>();

            CreateScriptEntity(entity);

            EntityInstance instance(GetEntityInheritClass(scrc.scriptClassName)->classID, entity);
            script_data->entityInstances[entity.GetPaperID()] = instance;


            for (const auto& [fieldID, fieldBuffer] : classFieldMapBuffer)
            {
                ManagedField* managedField = ScriptCache::GetManagedField(fieldID);
                GetEntityScriptInstance(entityID)->SetFieldValue(managedField, fieldBuffer.data);
            }
        }
    }

    bool ScriptEngine::ShouldReloadAssemblies()
    {
        return script_data->assembliesReloadSchedule;
    }

    void ScriptEngine::ScheduleAssembliesReload()
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

		
        const auto& scriptFields = ScriptClass(GetEntityInheritClass(sc.scriptClassName)).GetManagedFields();
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
            EntityInstance instance(GetEntityInheritClass(scrc.scriptClassName)->classID, entity);
            script_data->entityInstances[entity.GetPaperID()] = instance;

            //apply class variables defined in editor
            if (script_data->entityFieldStorage.contains(entity.GetPaperID()))
                for (const auto& fieldStorage : GetActiveEntityFieldStorage(entity))
                    fieldStorage->SetRuntimeInstance(GetEntityScriptInstance(entity.GetPaperID()));

            instance.InvokeOnCreate();
        }
    }

    void ScriptEngine::OnDestroyEntity(Entity entity)
    {
        CORE_ASSERT(script_data->entityInstances.contains(entity.GetPaperID()), "");
        EntityInstance* entityInstance = GetEntityScriptInstance(entity.GetPaperID());

    	if (!entityInstance) return;

        entityInstance->InvokeOnDestroy();

        //remove runtime instance of field storages
        if (script_data->entityFieldStorage.contains(entity.GetPaperID()))
            for (const auto& fieldStorage : GetActiveEntityFieldStorage(entity))
                fieldStorage->RemoveRuntimeInstance();

        script_data->entityInstances.erase(entity.GetPaperID());
    }

    void ScriptEngine::OnUpdateEntity(Entity entity, float dt)
    {
        CORE_ASSERT(script_data->entityInstances.contains(entity.GetPaperID()), "")

        GetEntityScriptInstance(entity.GetPaperID())->InvokeOnUpdate(dt);
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

    EntityInstance* ScriptEngine::GetEntityScriptInstance(PaperID entityID)
    {
        if (!script_data->entityInstances.contains(entityID)) return nullptr;
        return &script_data->entityInstances.at(entityID);
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

    std::unordered_map<PaperID, EntityInstance*> ScriptEngine::GetEntityInstances()
    {
        std::unordered_map<PaperID, EntityInstance*> entityInstances;
        for (auto& [entityID, entityInstance] : script_data->entityInstances)
        {
            entityInstances[entityID] = &entityInstance;
        }
        return entityInstances;
    }
    
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

    ScriptAssembly* ScriptEngine::GetCoreAssembly()
    {
        return &script_data->coreAssembly;
    }

    std::vector<ScriptAssembly*> ScriptEngine::GetAppAssemblies()
    {
        std::vector<ScriptAssembly*> appAssemblies;
        for (ScriptAssembly& assembly : script_data->appAssemblies)
        {
            appAssemblies.push_back(&assembly);
        }
        return appAssemblies;
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

    MonoObject* ScriptClass::Instantiate(bool showWarnings) const
    {
        uint32_t classFlags = managedClass->classFlags; //if sealed, class has no constructor or is not constructable i.e enums and static classes
        if (classFlags & MONO_TYPE_ATTR_SEALED)
        {
            if (showWarnings)
				LOG_CORE_WARN("Scripting: class is sealed and therefore it cannot be instatiated. '{}'", GetFullClassName());
            return nullptr;
        }
        if (!GetMethod(".ctor", 0))
        {
            if (showWarnings)
				LOG_CORE_WARN("Scripting: class has no default constructor and therefore it cannot be default instatiated. '{}'", GetFullClassName());
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

    std::vector<ManagedField*> ScriptClass::GetManagedFields() const
    {
        std::vector<ManagedField*> classFields;
        for (const CacheID fieldID : managedClass->fieldIDs)
        {
            ManagedField* field = ScriptCache::GetManagedField(fieldID);
            classFields.push_back(field);
        }
        return classFields;
    }

    ManagedField* ScriptClass::GetManagedField(const std::string& fieldName) const
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

    //
    //  EntityInstance
    //
    EntityInstance::EntityInstance(const CacheID entityScriptClassID, Entity entity)
        : ScriptInstance(entityScriptClassID), scriptClass(ScriptClass(entityScriptClassID))
    {
        CORE_ASSERT(ScriptClass(managedClassID).IsSubclassOf(script_data->entityClass), "Class must be a subclass of entity");

        LoadMethods();

        //constructor
        PaperID entityID = entity.GetPaperID();
        void* param = &entityID;
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
