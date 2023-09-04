#include "Engine.h"
#include "ScriptFieldStorage.h"

#include "ScriptEngine.h"
#include "ScriptUtils.h"

namespace Paper
{

	ScriptFieldStorage::ScriptFieldStorage(ManagedField* managedField)
		: managedField(managedField)
	{
	}

	void ScriptFieldStorage::SetRuntimeInstance(EntityInstance* instance)
	{
		runtimeInstance = instance;
		SetRuntimeFieldValue(data.data);
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
			Buffer valueBuffer;
			GetRuntimeFieldValue(valueBuffer);

			std::string value((char*)valueBuffer.data, valueBuffer.size / sizeof(char));
			valueBuffer.Release();
			return value;
		}

		if (data.size == 0)
			return std::string();

		std::string val = std::string((const char*)data.data);
		return val;
	}

	template <>
	void ScriptFieldStorage::SetValue<std::string>(const std::string& value, bool onlyBuffer)
	{
		if (runtimeInstance && !onlyBuffer)
		{
			//Buffer valueBuffer;
			//valueBuffer.Allocate(value.size() + 1);
			//valueBuffer.Nullify();
			//valueBuffer.Write(value.data(), value.size());
			SetRuntimeFieldValue(value.c_str());
			//valueBuffer.Release();
			return;
		}

		if (data.size <= value.size() * sizeof(char))
		{
			data.Release();
			data.Allocate((value.length() * 2) * sizeof(char));
		}

		data.Nullify();
		memcpy(data.data, value.c_str(), value.length() * sizeof(char));
	}

	Buffer ScriptFieldStorage::GetValueBuffer() const
	{
		if (runtimeInstance == nullptr)
			return data;

		Buffer result;
		GetRuntimeFieldValue(result);
		return result;
	}

	void ScriptFieldStorage::SetValueBuffer(const Buffer& buffer)
	{
		if (runtimeInstance != nullptr)
			SetRuntimeFieldValue(buffer.data);
		else
			data = Buffer::Copy(buffer);
	}

	void ScriptFieldStorage::GetRuntimeFieldValue(Buffer& outBuffer) const
	{
		outBuffer = runtimeInstance->GetFieldValue(managedField);
	}

	void ScriptFieldStorage::SetRuntimeFieldValue(const void* value) const
	{
		//if (scriptField->type != ScriptFieldType::String && value.size > 0)
		if (!managedField->isStatic)
			runtimeInstance->SetFieldValue(managedField, value);
	}
}
