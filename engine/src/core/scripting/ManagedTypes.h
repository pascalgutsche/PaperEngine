#pragma once
#include "generic/Buffer.h"

extern "C"
{
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoDomain MonoDomain;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoType MonoType;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoProperty MonoProperty;
	typedef struct _MonoString MonoString;
}

namespace Paper
{
	using CacheID = uint32_t;
	using ScriptFieldFlags = uint32_t;

	class ScriptAssembly;

	enum class ScriptFieldType
	{
		None = 0, Invalid, Not_Supported,
		Bool,
		Char, UChar,        // 1 byte
		Int16, UInt16,      // 2 byte
		Int32, UInt32,          // 4 byte
		Int64, UInt64,        // 8 byte
		Float, Double,      // 4, 8 byte
		String,
		Vec2, Vec3, Vec4,
		PaperID,
		Entity,

		//Components
		DataComponent, TransformComponent,
		SpriteComponent, LineComponent, TextComponent,
		CameraComponent, ScriptComponent
	};

	enum class ScriptFieldFlag
	{
		None = 0,
		Static = BIT(1),
		Readonly = BIT(2),
		Private = BIT(3),
		Internal = BIT(4),
		Protected = BIT(5),
		Public = BIT(6)
	};

	struct ManagedClass
	{
		CacheID classID = -1;
		std::string classNameSpace = "";
		std::string className = "";
		std::string fullClassName = "";
		uint32_t classFlags = 0;

		std::vector<uint32_t> fieldIDs;
		std::vector<uint32_t> methodIDs;

		MonoClass* monoClass = nullptr;

		ScriptAssembly* assembly = nullptr;

		bool operator==(const ManagedClass& other) const
		{
			return classID == other.classID;
		}

		bool operator!=(const ManagedClass& other) const
		{
			return !(*this == other);
		}
	};

	struct ManagedMethod
	{
		CacheID methodID = -1;
		std::string methodName = "";
		uint32_t methodFlags = 0;

		bool isVirtual = false;
		bool isStatic = false;
		uint32_t parameterCount = 0;

		MonoMethod* monoMethod = nullptr;

		uint32_t classID = -1;
		ScriptAssembly* assembly = nullptr;

		bool operator==(const ManagedMethod& other) const
		{
			return methodID == other.methodID;
		}

		bool operator!=(const ManagedMethod& other) const
		{
			return !(*this == other);
		}
	};

	struct ManagedField
	{
		CacheID fieldID = -1;
		std::string fieldName = "";
		uint32_t fieldFlags = 0;

		bool isStatic = false;
		bool isProperty = false;
		ScriptFieldType fieldType = ScriptFieldType::None;
		ScriptFieldFlags accessibilityFlags = 0;

		Buffer initialFieldValue;

		void* monoField = nullptr;

		uint32_t monoFieldSize = 0;

		uint32_t classID = -1;
		ScriptAssembly* assembly = nullptr;

		bool operator==(const ManagedField& other) const
		{
			return fieldID == other.fieldID;
		}

		bool operator!=(const ManagedField& other) const
		{
			return !(*this == other);
		}
	};
}
