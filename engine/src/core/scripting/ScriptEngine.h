#pragma once
#include "Engine.h"
#include "ScriptUtils.h"
#include "ScriptField.h"

#include "generic/Buffer.h"

#include "generic/Entity.h"

namespace Paper
{
	using EntityFieldStorage = std::vector<Shr<ScriptFieldStorage>>;

	class ScriptClass
	{
	public:
		ScriptClass(const std::string& classNameSpace, const std::string& className, MonoImage* monoImage = nullptr);

		MonoObject* Instantiate() const;
		MonoMethod* GetMethod(const std::string& methodName, uint32_t paramCount) const;
		void InvokeMethod(MonoObject* monoObject, MonoMethod* monoMethod, void** params = nullptr) const;

		const std::vector<ScriptField>& GetFields() { return fields; }

		bool IsSubclassOf(const Shr<ScriptClass>& scriptClass) const;

	private:
		std::string classNameSpace;
		std::string className;

		std::vector<ScriptField> fields;

		MonoClass* monoClass = nullptr;

		void InitFieldMap();

		friend class ScriptEngine;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(const Shr<ScriptClass>& scriptClass);

		const Buffer& GetFieldValue(const ScriptField& scriptField) const;

		template <typename T>
		void SetFieldValue(const ScriptField& scriptField, T& val)
		{
			SetFieldValueVoidPtr(scriptField, &val);
		};
		void SetFieldValueVoidPtr(const ScriptField& scriptField, const void* val) const;

		Shr<ScriptClass> GetScriptClass() const { return scriptClass; }

	protected:
		Shr <ScriptClass> scriptClass;
		MonoObject* monoInstance = nullptr;

	private:
		ScriptInstance(MonoObject* monoObject);
		const Buffer& GetFieldValueInternal(const ScriptField& scriptField, ScriptClass* scriptClass) const;


		

		friend class ScriptClass;
	};

	class EntityInstance : public ScriptInstance
	{
	public:
		EntityInstance(const Shr<ScriptClass>& scriptClass, Entity entity);

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
		static void Shutdown();

		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath);

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static void CreateScriptEntity(Entity entity);
		static void DestroyScriptEntity(Entity entity);

		static void OnCreateEntity(Entity entity);
		static void OnDestroyEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, float dt);

		static bool EntityClassExists(const std::string& fullClassName);
		static Shr<ScriptClass> GetEntityClass(const std::string& fullClassName);

		static Scene* GetSceneContext();
		static Shr<EntityInstance> GetEntityScriptInstance(UUID entityUUID);
		static const EntityFieldStorage& GetEntityFieldStorage(UUID entityUUID);
		static MonoDomain* GetDomain();
		static MonoImage* GetAppAssemblyImage();
		static MonoImage* GetCoreAssemblyImage();
		static std::unordered_map<std::string, Shr<ScriptClass>>& GetEntityClasses();
		static std::unordered_map<UUID, Shr<EntityInstance>>& GetEntityInstances();
	private:
		static void InitMono();
		static void ShutdownMono();


		static void LoadAssemblyClasses(MonoAssembly* monoAssembly);
	};

    
}

