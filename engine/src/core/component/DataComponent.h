#pragma once
#include "Engine.h"

#include "utils/EntityID.h"
#include "serializer/Serializable.h"

namespace Paper
{
	struct DataComponent : Serializable
	{
		EntityID uuid = EntityID();
		std::string name = "[Entity]";

		std::vector<std::string> tags;

		DataComponent() = default;
		~DataComponent() override = default;

		DataComponent(const EntityID& uuid)
			: uuid(uuid) { }

		DataComponent(const std::string& name)
			: name(name) { }

		DataComponent(const EntityID& uuid, const std::string& name)
			: uuid(uuid), name(name) { }

		bool Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node& data) override;
	};
}

