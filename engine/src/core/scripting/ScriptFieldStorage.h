#pragma once
#include "ScriptEngine.h"
#include "ScriptUtils.h"
#include "ScriptCache.h"

#include "generic/Buffer.h"
#include "utils/PaperID.h"


namespace Paper
{
	class EntityInstance;

	class ScriptFieldStorage
	{
	public:
		ScriptFieldStorage(PaperID entityID, ManagedField* managedField);
		ScriptFieldStorage(PaperID entityID);

		void SetRuntimeInstance(EntityInstance* instance) const;
		void RemoveRuntimeInstance() const;

		static void ClearRuntimeMap();

		template <typename T>
		T GetValue(bool onlyBuffer = false) const
		{
			if (!managedField)
			{
				LOG_CORE_WARN("You are trying to Call ScriptFieldStorage::SetValue without constructing with Managedfield*");
				return T();
			}

			Buffer valueBuffer = GetRuntimeFieldValue();
			if (!onlyBuffer && valueBuffer)
			{
				T value = T();
				memcpy(&value, valueBuffer.data, valueBuffer.size);
				valueBuffer.Release();
				return value;
			}

			Buffer& buffer = ScriptCache::GetFieldStorage(entityID, managedField);
			if (!buffer) return T();
			return buffer.Read<T>();
		}

		template<>
		std::string GetValue(bool onlyBuffer) const;

		template <typename T>
		void SetValue(const T& value, bool onlyBuffer = false)
		{
			if (!managedField)
			{
				LOG_CORE_WARN("You are trying to Call ScriptFieldStorage::SetValue without constructing with Managedfield*");
				return;
			}

			Buffer& buffer = ScriptCache::GetFieldStorage(entityID, managedField);
			if (!buffer)
			{
				buffer.Allocate(sizeof(T));
				buffer.Nullify();
			}
			buffer.Write(&value, sizeof(T));

			if (!onlyBuffer)
				SetRuntimeFieldValue();

		}

		template<>
		void SetValue<std::string>(const std::string& value, bool onlyBuffer);

		ManagedField* GetField() const { return managedField; }

	private:
		PaperID entityID = PaperID(0);
		ManagedField* managedField = nullptr;

		Buffer GetRuntimeFieldValue() const;
		void SetRuntimeFieldValue() const;

		friend class ScriptEngine;

		static inline std::unordered_map<PaperID, EntityInstance*> instanceMap;
	};
}
