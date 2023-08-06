#pragma once
#include "Engine.h"

#include "ScriptUtils.h"
#include "generic/Buffer.h"

#include "generic/Entity.h"

namespace Paper
{
	using ScriptFieldFlags = uint32_t;

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

	enum class ScriptFieldFlag
	{
		None = -1,
		Static = BIT(0),
		Readonly = BIT(1),
		Private = BIT(2),
		Internal = BIT(3),
		Protected = BIT(4),
		Public = BIT(5)
	};
	

	struct ScriptField
	{
		std::string name;
		ScriptFieldType type;
		ScriptFieldFlags flags;

		uint32_t typeSize;
		Buffer InitialFieldVal;

		MonoClassField* monoField;

		bool HasFlag(ScriptFieldFlag flag) const { return flags & (uint32_t)flag; }

		bool IsWritable() const { return !HasFlag(ScriptFieldFlag::Readonly) && HasFlag(ScriptFieldFlag::Public); }
	};

	class ScriptClass
	{
	public:
		ScriptClass(const std::string& classNameSpace, const std::string& className, MonoImage* monoImage = nullptr);

		MonoObject* Instantiate() const;
		MonoMethod* GetMethod(const std::string& methodName, uint32_t paramCount) const;
		void InvokeMethod(MonoObject* monoObject, MonoMethod* monoMethod, void** params = nullptr) const;

		const std::unordered_map<std::string, ScriptField>& GetFields() { return fields; }

		bool IsSubclassOf(const Shr<ScriptClass>& scriptClass) const;

		static Shr<ScriptClass> Create(const std::string& classNameSpace, const std::string& className, MonoImage* monoImage = nullptr);
	private:
		std::string classNameSpace;
		std::string className;

		std::unordered_map<std::string, ScriptField> fields;

		MonoClass* monoClass = nullptr;

		void InitFieldMap();

		friend class ScriptEngine;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(const Shr<ScriptClass>& scriptClass);
		ScriptInstance(MonoObject* monoObject);

		template <typename T>
		T GetFieldValue(const std::string& fieldName)
		{
			char* buffer[sizeof(T)];
			if (GetFieldValueInternal(fieldName, buffer))
				return *(T*)buffer;
			return T();
		};

		template <typename T>
		void SetFieldValue(const std::string& fieldName, T& val)
		{
			SetFieldValueInternal(fieldName, &val);
		};

		Shr<ScriptClass> GetScriptClass() const { return scriptClass; }

	protected:
		Shr <ScriptClass> scriptClass;
		MonoObject* monoInstance = nullptr;

	private:
		void GetFieldValueInternal(const ScriptField& field, void* buffer) const;
		bool GetFieldValueInternal(const std::string& fieldName, void* buffer) const;

		void SetFieldValueInternal(const ScriptField& field, void* val) const;
		void SetFieldValueInternal(const std::string& fieldName, void* val) const;


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

