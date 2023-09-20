#include "Engine.h"
#include "ScriptFieldStorage.h"

#include "ScriptCache.h"
#include "ScriptEngine.h"
#include "ScriptUtils.h"

namespace Paper
{

	ScriptFieldStorage::ScriptFieldStorage(PaperID entityID, ManagedField* managedField)
		: entityID(entityID), managedField(managedField)
	{
	}

	ScriptFieldStorage::ScriptFieldStorage(PaperID entityID)
		: entityID(entityID)
	{
	}

	void ScriptFieldStorage::SetRuntimeInstance(EntityInstance* instance) const
	{
		instanceMap[entityID] = instance;
		SetRuntimeFieldValue();
	}

	void ScriptFieldStorage::RemoveRuntimeInstance() const
	{
		instanceMap.erase(entityID);
	}

	void ScriptFieldStorage::ClearRuntimeMap()
	{
		instanceMap.clear();
	}

	template <>
	std::string ScriptFieldStorage::GetValue<std::string>(bool onlyBuffer) const
	{
		if (!managedField)
		{
			LOG_CORE_WARN("You are trying to Call ScriptFieldStorage::SetValue<std::string> without constructing with Managedfield*");
			return "";
		}

		Buffer buffer;
		if (!onlyBuffer)
			buffer = GetRuntimeFieldValue();
		if (!buffer)
		{
			Buffer& storageBuffer = ScriptCache::GetFieldStorage(entityID, managedField);
			buffer = Buffer::Copy(storageBuffer);
		}

		if (buffer.size == 0)
			return {};

		std::string val = std::string((const char*)buffer.data);
		buffer.Release();

		return val;
	}

	template <>
	void ScriptFieldStorage::SetValue<std::string>(const std::string& value, bool onlyBuffer)
	{
		if (!managedField) 
		{
			LOG_CORE_WARN("You are trying to Call ScriptFieldStorage::SetValue<std::string> without constructing with Managedfield*");
			return;
		}


		Buffer& buffer = ScriptCache::GetFieldStorage(entityID, managedField);
		if (buffer.size <= value.size() * sizeof(char))
		{
			buffer.Release();
			buffer.Allocate((value.length() * 2) * sizeof(char));
		}

		buffer.Nullify();
		memcpy(buffer.data, value.c_str(), value.length() * sizeof(char));

		if (!onlyBuffer)
			SetRuntimeFieldValue();
	}

	Buffer ScriptFieldStorage::GetRuntimeFieldValue() const
	{
		if (!instanceMap.contains(entityID)) return {};
		const EntityInstance* runtimeInstance = instanceMap.at(entityID);
		return runtimeInstance->GetFieldValue(managedField);
	}

	void ScriptFieldStorage::SetRuntimeFieldValue() const
	{
		if (!instanceMap.contains(entityID)) return;
		const EntityInstance* runtimeInstance = instanceMap.at(entityID);
		if (managedField)
		{
			Buffer& buffer = ScriptCache::GetFieldStorage(entityID, managedField);
			runtimeInstance->SetFieldValue(managedField, buffer.data);
			return;
		}
		const auto managedFields = ScriptClass(runtimeInstance->GetManagedClass()).GetManagedFields();
		for (ManagedField* field : managedFields)
		{
			const Buffer& buffer = ScriptCache::GetFieldStorage(entityID, field);
			runtimeInstance->SetFieldValue(field, buffer.data);
		}
	}
}
