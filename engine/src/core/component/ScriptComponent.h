#pragma once
#include "serializer/Serializable.h"

namespace Paper
{
	struct ScriptComponent : Serializable
	{
		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;

		std::string name;

		bool Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node& data) override;
	};
}
