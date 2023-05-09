#pragma once
#include "_Core.h"
#include "utility.h"

#include "event/Event.h"
#include "generic/Entity.h"

namespace core
{

	class EntityEvent : public Event
	{
	public:
		EntityEvent() = default;
		EntityEvent(Entity* gameObject)
			: gameObject(gameObject) { }

		inline Entity* GetEntity() const { return gameObject; }

		inline std::string ToString() const override {
			std::stringstream string;
			string << "EntityEvent: Name: " << gameObject->GetName() << " | CoreID: " << gameObject->GetCoreID();
			return string.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryEntity | EventCategoryGame);

		EVENT_CLASS_TYPE(EntityPressed);

	protected:
		Entity* gameObject = nullptr;
	};

	class EntityPressedEvent : public EntityEvent {
	public:
		EntityPressedEvent() = default;
		EntityPressedEvent(Entity* gm)
			: EntityEvent(gm) { }


		inline std::string ToString() const override {
			std::stringstream string;
			string << "EntityPressedEvent: Name: " << gameObject->GetName() << " | ID: " << gameObject->GetCoreID();
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
			string << "EntityReleasedEvent: Name: " << gameObject->GetName() << " | ID: " << gameObject->GetCoreID();
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
			string << "EntityHoverBeginEvent: Name: " << gameObject->GetName() << " | ID: " << gameObject->GetCoreID();
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
			string << "EntityHoverEndEvent: Name: " << gameObject->GetName() << " | ID: " << gameObject->GetCoreID();
			return string.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryEntity | EventCategoryGame);

		EVENT_CLASS_TYPE(EntityHoverEnd);
	};
}
