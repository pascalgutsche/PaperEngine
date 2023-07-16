#pragma once
#include "Engine.h"

#include "utils/UUID.h"
#include "serializer/Serializable.h"

namespace Paper
{
	struct DataComponent : Serializable
	{
		UUID uuid = UUID();
		std::string name = "[Entity]";

		std::vector<std::string> tags;

		DataComponent() = default;
		~DataComponent() override = default;

		DataComponent(const UUID& uuid)
			: uuid(uuid) { }

		DataComponent(const std::string& name)
			: name(name) { }

		DataComponent(const UUID& uuid, const std::string& name)
			: uuid(uuid), name(name) { }

		bool Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node& data) override;
	};
}

