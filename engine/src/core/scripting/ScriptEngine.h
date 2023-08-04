#pragma once
#include "Engine.h"

#include "ScriptUtils.h"

#include "generic/Entity.h"

namespace Paper
{
	using ScriptFieldVisibilityBitMap = uint32_t;

	enum class ScriptFieldType
	{
		None = 0, Invalid, Not_Supported,
		Bool,
		Char, UChar,        // 1 byte
		Short, UShort,      // 2 byte
		Int, UInt,          // 4 byte
		Long, ULong,        // 8 byte
		Float, Double,      // 4, 8 byte
		String,
		Vec2, Vec3, Vec4,
		Entity
	};

	enum class ScriptFieldVisibility
	{
		None = 0,
		Private = (1 << 0),
		Internal = (1 << 1),
		Protected = (1 << 2),
		Public = (1 << 3)
	};
	

	struct ScriptField
	{
		std::string name;
		ScriptFieldType type;
		ScriptFieldVisibilityBitMap visibility;

		MonoClassField* monoField;
	};

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

		friend class ScriptEngine;
	};

	class EntityInstance
	{
	public:
		EntityInstance(const Shr<ScriptClass>& scriptClass, Entity entity);

		void InvokeOnCreate() const;
		void InvokeOnDestroy() const;
		void InvokeOnUpdate(float dt) const;

		template <typename T>
		T GetFieldValue(const std::string& fieldName)
		{
			char* buffer[sizeof(T)];
			if (GetFieldValueVoid(fieldName, buffer))
				return *(T*)buffer;
			return T();
		};

		template <typename T>
		void SetFieldValue(const std::string& fieldName, T& val)
		{
			SetFieldValueVoid(fieldName, &val);
		};

		Shr<ScriptClass> GetScriptClass() const { return scriptClass; }

	private:
		Shr <ScriptClass> scriptClass;
		MonoObject* monoInstance = nullptr;

		MonoMethod* constructor = nullptr;
		MonoMethod* onCreateMethod = nullptr;
		MonoMethod* onDestroyMethod = nullptr;
		MonoMethod* onUpdateMethod = nullptr;

		bool GetFieldValueVoid(const std::string& fieldName, void* buffer);
		void SetFieldValueVoid(const std::string& fieldName, void* val);
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
		static Shr<ScriptClass> GetEntityClass(const std::string& fullClassName);

		static Scene* GetSceneContext();
		static Shr<EntityInstance> GetEntityScriptInstance(UUID entityUUID);
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

