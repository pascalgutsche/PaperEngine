#pragma once
#include "Engine.h"

namespace engine
{
	struct DataComponent
	{
		uuid uuid;
		std::string name;

		std::vector<std::string> tags;

		DataComponent(uuids::uuid uuid, std::string name = "Entity")
			: uuid(uuid), name(name) { }

		DataComponent(std::string name = "Entity")
		{
			this->name = name;
			this->uuid = uuid_system_generator{}();
			CORE_ASSERT(!uuid.is_nil(), "");
			CORE_ASSERT(uuid.version() == uuids::uuid_version::random_number_based, "");
			CORE_ASSERT(uuid.variant() == uuids::uuid_variant::rfc, "");
		}
	};
}

