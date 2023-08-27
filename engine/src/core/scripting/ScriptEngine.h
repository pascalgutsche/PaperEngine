#pragma once
#include "Engine.h"

#include "ScriptUtils.h"
#include "ScriptField.h"

#include "generic/Buffer.h"

#include "generic/Entity.h"

namespace Paper
{
	using EntityFieldStorage = std::vector<Shr<ScriptFieldStorage>>;

	class ScriptAssembly;

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(CacheID classID);
		ScriptClass(ManagedClass* managedClass);
		~ScriptClass();

		MonoObject* Instantiate() const;

		template <typename... ConstructorArgs>
		MonoObject* InstantiateParams(ConstructorArgs&&... args) const
		{
			MonoObject* monoInstance = Instantiate();

			constexpr size_t argsCount = sizeof...(args);
			ManagedMethod* ctor = GetMethod(".ctor", argsCount);

			if constexpr (argsCount > 0)
			{
				if (!ctor)
				{
					LOG_CORE_ERROR("Could not find a constructor with {} parameters from class {}.", argsCount, GetFullClassName());
					return monoInstance;
				}

				void* data[] = { &args... };
				InvokeMethod(monoInstance, ctor, data);
			}

			return monoInstance;
		};

		ManagedMethod* GetMethod(const std::string& methodName, uint32_t paramCount) const;
		void InvokeMethod(MonoObject* monoObject, const ManagedMethod* managedMethod, void** params = nullptr) const;

		std::string GetFullClassName() const;
		MonoClass* GetMonoClass() const;
		ManagedClass* GetManagedClass() const;
		std::vector<ManagedField*> GetFields() const;
		ManagedField* GetField(const std::string& fieldName) const;
		ScriptAssembly* GetScriptAssembly() const;

		bool IsSubclassOf(const ScriptClass& scriptClass) const;
		bool IsSubclassOf(const CacheID classID) const { return IsSubclassOf(ScriptClass(classID)); }

	private:
		CacheID id = 0;
		ManagedClass* managedClass = nullptr;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance() = default;
		ScriptInstance(CacheID classID);
		ScriptInstance(ManagedClass* managedClass);
		ScriptInstance(const ScriptClass& scriptClass);

		Buffer GetFieldValue(ManagedField* managedField) const;
		void SetFieldValue(ManagedField* managedField, const void* value) const;

		ManagedClass* GetManagedClass() const;
		MonoObject* GetMonoInstance() const;
	protected:
		CacheID managedClassID = 0;
		ManagedClass* managedClass = nullptr;
		MonoObject* monoInstance = nullptr;
	};

	class EntityInstance : public ScriptInstance
	{
	public:
		EntityInstance() = default;
		EntityInstance(CacheID entityScriptClassID, Entity entity);

		void LoadMethods();

		void InvokeOnCreate() const;
		void InvokeOnDestroy() const;
		void InvokeOnUpdate(float dt) const;

	private:
		ManagedMethod* constructor = nullptr;
		ManagedMethod* onCreateMethod = nullptr;
		ManagedMethod* onDestroyMethod = nullptr;
		ManagedMethod* onUpdateMethod = nullptr;

		//convenience
		ScriptClass scriptClass;
	};

	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown(bool appClose = false);

		//static void LoadAssembly(const std::filesystem::path& filepath);
		//static void LoadAppAssembly(const std::filesystem::path& filepath);
		static void ReloadAppAssembly();

		static bool ShouldReloadAppAssembly();
		static void ScheduleAssemblyReload();

		static void SetToRootDomain();

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static void CreateScriptEntity(Entity entity);
		static void DestroyScriptEntity(Entity entity);

		static void OnCreateEntity(Entity entity);
		static void OnDestroyEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, float dt);


		static Scene* GetSceneContext();
		static EntityInstance* GetEntityScriptInstance(PaperID entityUUID);
		static const EntityFieldStorage& GetActiveEntityFieldStorage(Entity entity);
		static std::unordered_map<CacheID, EntityFieldStorage>& GetEntityFieldStorage(Entity entity);
		static MonoDomain* GetDomain();
		static Shr<ScriptAssembly> GetCoreAssembly();
		static std::vector<Shr<ScriptAssembly>>& GetAppAssemblies();

		static ManagedClass* GetEntityClass();
		static void SetEntityClass(ManagedClass* managedEntityClass);

		static std::vector<ManagedClass*> GetScriptClasses();

		static std::vector<ManagedClass*> GetEntityInheritClasses();
		static ManagedClass* GetEntityInheritClass(const std::string& fullClassName);
		static bool EntityInheritClassExists(const std::string& fullClassName);

		static std::unordered_map<PaperID, EntityInstance*> GetEntityInstances();
	private:
		static void InitMono();
		static void ShutdownMono(bool appClose);

		static EntityFieldStorage& GetActiveEntityFieldStorageInternal(Entity entity);

		//static void LoadAssemblyClasses(MonoAssembly* monoAssembly);
	};

    
}

