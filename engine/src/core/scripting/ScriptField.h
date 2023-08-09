#pragma once
#include "ScriptUtils.h"

#include "generic/Buffer.h"


namespace Paper
{
	class EntityInstance;

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
		std::string name;
		ScriptFieldType type;
		ScriptFieldFlags flags;

		uint32_t typeSize;
		Buffer initialFieldVal;

		MonoClassField* monoField;

		ScriptField() = default;

		ScriptField(const ScriptField& other)
		{
			name = other.name;
			type = other.type;
			flags = other.flags;
			typeSize = other.typeSize;
			initialFieldVal = Buffer(other.initialFieldVal);
			monoField = other.monoField;
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
		T& GetValue(bool onlyBuffer = false)
		{
			if (runtimeInstance && !onlyBuffer)
			{
				Buffer valueBuffer = GetRuntimeFieldValue();
				T value = T();
				memcpy(&value, valueBuffer.data, valueBuffer.size);
				valueBuffer.Release();
				return value;
			}

			return data.Read<T>();
		}

		template <typename T>
		void SetValue(const T& value, bool onlyBuffer = false)
		{
			if (runtimeInstance && !onlyBuffer)
			{
				SetRuntimeFieldValue(&value);
				return;
			}

			data.Write(&value, sizeof(T));
		}

		const ScriptField& GetField() const { return *scriptField; }

	private:
		const ScriptField* scriptField = nullptr;
		Buffer data = Buffer::Copy(scriptField->initialFieldVal);

		const Buffer& GetRuntimeFieldValue() const;
		void SetRuntimeFieldValue(const void* value) const;

		Shr<EntityInstance> runtimeInstance = nullptr;
	};
}