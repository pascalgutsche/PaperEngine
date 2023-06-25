#pragma once
#include "Engine.h"
#include "utility.h"

#include "event/Event.h"
#include "generic/Entity.h"

#include "component/DataComponent.h"

namespace ppr
{

	class EntityEvent : public Event
	{
	public:
		EntityEvent() = default;
		EntityEvent(Entity* entity)
			: entity(entity) { }

		inline Entity* GetEntity() const { return entity; }

		inline std::string ToString() const override {
			std::stringstream string;
			string << "EntityEvent: Name: " << entity->GetComponent<DataComponent>().name << " | CoreID: " << entity->GetUUID();
			return string.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryEntity | EventCategoryGame);

		EVENT_CLASS_TYPE(EntityPressed);

	protected:
		Entity* entity = nullptr;
	};

	class EntityPressedEvent : public EntityEvent {
	public:
		EntityPressedEvent() = default;
		EntityPressedEvent(Entity* gm)
			: EntityEvent(gm) { }


		inline std::string ToString() const override {
			std::stringstream string;
			string << "EntityPressedEvent: Name: " << entity->GetName() << " | ID: " << entity->GetUUID();
			return string.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryEntity | EventCategoryGame);

		EVENT_CLASS_TYPE(EntityPressed);
	};

	class EntityReleasedEvent : public EntityEvent {
	public:
		EntityReleasedEvent() = default;
		EntityReleasedEvent(Entity* gm)
			: EntityEvent(gm) { }

		std::string ToString() const override {
			std::stringstream string;
			string << "EntityReleasedEvent: Name: " << entity << " | ID: " << entity->GetUUID();
			return string.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryEntity | EventCategoryGame);

		EVENT_CLASS_TYPE(EntityReleased);
	};

	class EntityHoverBeginEvent : public EntityEvent {
	public:
		EntityHoverBeginEvent() = default;
		EntityHoverBeginEvent(Entity* gm)
			: EntityEvent(gm) { }

		std::string ToString() const override {
			std::stringstream string;
			string << "EntityHoverBeginEvent: Name: " << entity->GetName() << " | ID: " << entity->GetUUID();
			return string.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryEntity | EventCategoryGame);

		EVENT_CLASS_TYPE(EntityHoverBegin);
	};

	class EntityHoverEndEvent : public EntityEvent {
	public:
		EntityHoverEndEvent() = default;
		EntityHoverEndEvent(Entity* gm)
			: EntityEvent(gm) { }

		std::string ToString() const override {
			std::stringstream string;
			string << "EntityHoverEndEvent: Name: " << entity->GetName() << " | ID: " << entity->GetUUID();
			return string.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryEntity | EventCategoryGame);

		EVENT_CLASS_TYPE(EntityHoverEnd);
	};
}
