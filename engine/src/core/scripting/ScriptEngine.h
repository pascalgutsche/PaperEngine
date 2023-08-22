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
		: public std::enable_shared_from_this<ScriptClass>
	{
	public:
		ScriptClass(CacheID classID);

		MonoObject* Instantiate() const;

		template <typename... ConstructorArgs>
		MonoObject* InstantiateParams(ConstructorArgs&&... args) const
		{
			MonoObject* monoInstance = Instantiate();

			constexpr size_t argsCount = sizeof...(args);
			MonoMethod* ctor = GetMethod(".ctor", argsCount);

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

		MonoMethod* GetMethod(const std::string& methodName, uint32_t paramCount) const;
		void InvokeMethod(MonoObject* monoObject, MonoMethod* monoMethod, void** params = nullptr) const;

		std::string GetFullClassName() const;
		MonoClass* GetMonoClass() const;
		const std::vector<ScriptField>& GetFields() const { return std::vector<ScriptField>(); }
		ScriptField* GetField(const std::string& fieldName) const;
		const ScriptAssembly& GetScriptAssembly() const { return *managedClass->assembly; }

		bool IsSubclassOf(const ScriptClass& scriptClass) const;

	private:
		CacheID id = 0;
		ManagedClass* managedClass = nullptr;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(const Shr<ScriptClass>& scriptClass);

		void GetFieldValue(const ScriptField& scriptField, Buffer& outBuffer) const;
		void SetFieldValue(const ScriptField& scriptField, const void* value) const;

		Shr<ScriptClass> GetScriptClass() const { return scriptClass; }
		MonoObject* GetMonoInstance() const { return monoInstance; }
	protected:
		Shr <ScriptClass> scriptClass = nullptr;
		MonoObject* monoInstance = nullptr;

	private:
		ScriptInstance(MonoObject* instance);

		void SetScriptClass(Shr<ScriptClass> newScriptClass);
		void GetFieldValueInternal(const ScriptField& scriptField, Buffer& outBuffer) const;
		void SetFieldValueInternal(const ScriptField& scriptField, const void* value) const;

		friend class ScriptUtils;
		friend class ScriptClass;
		friend class ScriptEngine;
	};

	class EntityInstance : public ScriptInstance
	{
	public:
		EntityInstance(const Shr<ScriptClass>& scriptClass, Entity entity);

		void LoadMethods();

		void InvokeOnCreate() const;
		void InvokeOnDestroy() const;
		void InvokeOnUpdate(float dt) const;

	private:
		MonoMethod* constructor = nullptr;
		MonoMethod* onCreateMethod = nullptr;
		MonoMethod* onDestroyMethod = nullptr;
		MonoMethod* onUpdateMethod = nullptr;
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
		static Shr<EntityInstance> GetEntityScriptInstance(PaperID entityUUID);
		static const EntityFieldStorage& GetActiveEntityFieldStorage(Entity entity);
		static std::unordered_map<Shr<ScriptClass>, EntityFieldStorage>& GetEntityFieldStorage(Entity entity);
		static MonoDomain* GetDomain();
		static Shr<ScriptAssembly> GetCoreAssembly();
		static std::vector<Shr<ScriptAssembly>>& GetAppAssemblies();

		static Shr<ScriptClass> GetEntityClass();
		static void SetEntityClass(Shr<ScriptClass> entityClass);

		static std::unordered_map<std::string, Shr<ScriptClass>> GetScriptClasses();

		static std::unordered_map<std::string, Shr<ScriptClass>> GetEntityInheritClasses();
		static Shr<ScriptClass> GetEntityInheritClass(const std::string& fullClassName);
		static bool EntityInheritClassExists(const std::string& fullClassName);

		static std::unordered_map<PaperID, Shr<EntityInstance>>& GetEntityInstances();
	private:
		static void InitMono();
		static void ShutdownMono(bool appClose);

		static EntityFieldStorage& GetActiveEntityFieldStorageInternal(Entity entity);

		//static void LoadAssemblyClasses(MonoAssembly* monoAssembly);
	};

    
}

