#pragma once
#include "Serializable.h"

namespace Paper
{
	class ScriptClass;

	struct ScriptComponent : Serializable
	{
		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;

		std::string scriptClassName;

		bool Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node& data) override;
	};
}
