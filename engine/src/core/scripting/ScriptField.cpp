#include "Engine.h"
#include "ScriptField.h"
#include "ScriptEngine.h"


namespace Paper
{
	ScriptFieldStorage::ScriptFieldStorage(const ScriptField* scriptField)
		: scriptField(scriptField)
	{
	}

	void ScriptFieldStorage::SetRuntimeInstance(Shr<EntityInstance> instance)
	{
		runtimeInstance = instance;
		runtimeInstance->SetFieldValueVoidPtr(*scriptField, data.data);
	}

	void ScriptFieldStorage::RemoveRuntimeInstance()
	{
		runtimeInstance = nullptr;
	}

	template <>
	std::string ScriptFieldStorage::GetValue<std::string>(bool onlyBuffer) const
	{
		if (runtimeInstance != nullptr)
		{
			Buffer valueBuffer = GetRuntimeFieldValue();

			std::string value((char*)valueBuffer.data, valueBuffer.size / sizeof(char));
			valueBuffer.Release();
			return value;
		}
		std::string val = std::string((char*)data.data, data.size / sizeof(char));
		return val;
	}

	template <>
	void ScriptFieldStorage::SetValue<std::string>(const std::string& value, bool onlyBuffer)
	{
		if (runtimeInstance && !onlyBuffer)
		{
			SetRuntimeFieldValue(&value);
			return;
		}

		if (data.size <= value.length() * sizeof(char))
		{
			data.Release();
			data.Allocate((value.length() * 2) * sizeof(char));
		}

		data.Nullify();
		memcpy(data.data, value.c_str(), value.length() * sizeof(char));
	}

	const Buffer& ScriptFieldStorage::GetRuntimeFieldValue() const
	{
		return runtimeInstance->GetFieldValue(*scriptField);
	}

	void ScriptFieldStorage::SetRuntimeFieldValue(const void* value) const
	{
		runtimeInstance->SetFieldValueVoidPtr(*scriptField, value);
	}
}
