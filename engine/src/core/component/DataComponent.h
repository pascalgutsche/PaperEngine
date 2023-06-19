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
			static_assert(!uuid.is_nil());
			static_assert(uuid.version() == uuids::uuid_version::random_number_based);
			static_assert(uuid.variant() == uuids::uuid_variant::rfc);
		}
	};
}

