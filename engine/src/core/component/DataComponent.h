#pragma once
#include "Engine.h"

#include "utils/PaperID.h"
#include "Serializable.h"

namespace Paper
{
	struct DataComponent : Serializable
	{
		PaperID uuid = PaperID();
		std::string name = "[Entity]";

		std::vector<std::string> tags;

		DataComponent() = default;
		~DataComponent() override = default;

		DataComponent(const PaperID& uuid)
			: uuid(uuid) { }

		DataComponent(const std::string& name)
			: name(name) { }

		DataComponent(const PaperID& uuid, const std::string& name)
			: uuid(uuid), name(name) { }

		bool Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node& data) override;
	};
}

