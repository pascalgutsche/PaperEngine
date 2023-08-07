#pragma once
#include "serializer/Serializable.h"

namespace Paper
{
	class ScriptClass;

	struct ScriptComponent : Serializable
	{
		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;

		Shr<ScriptClass> scriptClass = nullptr;

		bool Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node& data) override;
	};
}
