#pragma once
#include "Engine.h"

#include "generic/Entity.h"

extern "C"
{
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoDomain MonoDomain;
	typedef struct _MonoImage MonoImage;
}

namespace Paper
{
	class Scene;

	class ScriptClass
	{
	public:
		ScriptClass(const std::string& classNameSpace, const std::string& className, MonoImage* monoImage = nullptr);

		MonoObject* Instantiate() const;
		MonoMethod* GetMethod(const std::string& methodName, uint32_t paramCount) const;
		void InvokeMethod(MonoObject* monoObject, MonoMethod* monoMethod, void** params = nullptr) const;

		bool IsSubclassOf(const Shr<ScriptClass>& scriptClass) const;

	private:
		std::string classNameSpace;
		std::string className;

		MonoClass* monoClass = nullptr;

		friend class ScriptEngine;
	};

	class EntityInstance
	{
	public:
		EntityInstance(const Shr<ScriptClass>& scriptClass, Entity entity);

		void InvokeOnCreate() const;
		void InvokeOnDestroy() const;
		void InvokeOnUpdate(float dt) const;

	private:
		Shr <ScriptClass> scriptClass;
		MonoObject* monoInstance = nullptr;

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

		static void OnCreateEntity(Entity entity);
		static void OnDestroyEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, float dt);

		static bool EntityClassExists(const std::string& fullClassName);

		static Scene* GetSceneContext();
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

