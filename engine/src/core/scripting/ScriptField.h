#pragma once
#include "ScriptUtils.h"

#include "generic/Buffer.h"
#include "utils/PaperID.h"


namespace Paper
{
	class EntityInstance;

	class ScriptFieldStorage
	{
	public:
		ScriptFieldStorage(ManagedField* scriptField);

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

		ManagedField* GetField() const { return managedField; }

	private:
		ManagedField* managedField = nullptr;
		Buffer data = Buffer::Copy(managedField->initialFieldValue);

		void GetRuntimeFieldValue(Buffer& outBuffer) const;
		void SetRuntimeFieldValue(const void* value) const;

		Shr<EntityInstance> runtimeInstance = nullptr;

		friend class ScriptEngine;
	};
}
