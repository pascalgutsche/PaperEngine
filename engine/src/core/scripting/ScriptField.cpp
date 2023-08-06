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

	const Buffer& ScriptFieldStorage::GetRuntimeFieldValue() const
	{
		return runtimeInstance->GetFieldValue(*scriptField);
	}

	void ScriptFieldStorage::SetRuntimeFieldValue(const void* value) const
	{
		runtimeInstance->SetFieldValueVoidPtr(*scriptField, value);
	}
}
