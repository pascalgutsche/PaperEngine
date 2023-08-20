#pragma once
#include "ScriptUtils.h"

#include "generic/Buffer.h"
#include "utils/PaperID.h"


namespace Paper
{
	class EntityInstance;

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


	struct ScriptField
	{
		std::string name = "";
		ScriptFieldType type = ScriptFieldType::None;
		ScriptFieldFlags flags = 0;

		uint32_t typeSize = 0;
		Buffer initialFieldVal;

		MonoClassField* monoField = nullptr;
		MonoProperty* monoProperty = nullptr;

		bool isProperty = false;
	private:
		std::string stringType = "";

		friend class ScriptUtils;
		friend class ScriptEngine;
	public:

		ScriptField() = default;

		ScriptField(const ScriptField& other)
		{
			name = other.name;
			type = other.type;
			flags = other.flags;
			typeSize = other.typeSize;
			initialFieldVal = Buffer(other.initialFieldVal);
			monoField = other.monoField;
			monoProperty = other.monoProperty;
			isProperty = other.isProperty;
			stringType = other.stringType;
		}

		bool HasFlag(ScriptFieldFlag flag) const { return flags & (uint32_t)flag; }

		bool IsWritable() const { return !HasFlag(ScriptFieldFlag::Readonly) && HasFlag(ScriptFieldFlag::Public); }

		bool operator==(const ScriptField& other) const
		{
			return other.monoField == monoField;
		}

		~ScriptField()
		{
			initialFieldVal.Release();
		}

		
	};

	class ScriptFieldStorage
	{
	public:
		ScriptFieldStorage(const ScriptField* scriptField);

		void SetRuntimeInstance(Shr<EntityInstance> instance);
		void RemoveRuntimeInstance();

		template <typename T>
		T GetValue(bool onlyBuffer = false) const
		{
			if (runtimeInstance && !onlyBuffer)
			{
				Buffer valueBuffer;
				GetRuntimeFieldValue(valueBuffer);
				T value = T();
				memcpy(&value, valueBuffer.data, valueBuffer.size);
				valueBuffer.Release();
				return value;
			}

			if (!data) return T();
			return data.Read<T>();
		}

		template<>
		std::string GetValue(bool onlyBuffer) const;

		template <typename T>
		void SetValue(const T& value, bool onlyBuffer = false)
		{
			if (runtimeInstance && !onlyBuffer)
			{
				Buffer val;
				val.Allocate(sizeof(T));
				SetRuntimeFieldValue(&value);
				return;
			}

			data.Write(&value, sizeof(T));
		}

		template<>
		void SetValue<std::string>(const std::string& value, bool onlyBuffer);

		Buffer GetValueBuffer() const;
		void SetValueBuffer(const Buffer& buffer);

		const ScriptField& GetField() const { return *scriptField; }

	private:
		const ScriptField* scriptField = nullptr;
		Buffer data = Buffer::Copy(scriptField->initialFieldVal);

		void GetRuntimeFieldValue(Buffer& outBuffer) const;
		void SetRuntimeFieldValue(const void* value) const;

		Shr<EntityInstance> runtimeInstance = nullptr;

		friend class ScriptEngine;
	};
}
