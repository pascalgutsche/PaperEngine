#pragma once
#include "Engine.h"

namespace ppr
{
	struct DataComponent
	{
		UUID uuid;
		std::string name;

		std::vector<std::string> tags;

		DataComponent(const UUID& uuid, const std::string& name = "Entity")
			: uuid(uuid), name(name) { }

		DataComponent(const std::string& name = "Entity")
		{
			this->name = name;
		}
	};
}

