#pragma once
#include "Engine.h"
#include "Transform.h"

namespace Paper
{
	class Object
	{
	public:
		Object(std::string name, const Transform& transform);
		virtual ~Object();

		void SetName(std::string name);

		std::string GetName();
		entity_id GetCoreID() const;

		Transform transform;

	protected:
		std::string name;
		entity_id coreID;
	};
}

