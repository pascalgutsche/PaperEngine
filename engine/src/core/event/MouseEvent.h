#pragma once
#include "Engine.h"
#include "utility.h"

#include "event/Event.h"

#include "ButtonCodes.h"

//MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled

namespace Paper {
	class MouseButtonPressedEvent final : public Event {
	private:
		MouseButton button;
		int mods;

	public:
		MouseButtonPressedEvent(MouseButton button, const int mods)
			:button(button), mods(mods) { }

		inline MouseButton GetButton() const { return button; }
		inline bool IsModPressed(Mod mod) const { return mods & (int)mod; }


		std::string ToString() const override {
			std::stringstream string;
			string << "MouseButtonPressedEvent: " << (int)button;
			return string.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryMouse | EventCategoryInput)

		EVENT_CLASS_TYPE(MouseButtonPressed)

	};

	class MouseButtonReleasedEvent final : public Event {
	private:
		MouseButton button;
		int mods;
	public:
		MouseButtonReleasedEvent(MouseButton button, const int mods)
			:button(button), mods(mods) { }

		inline MouseButton GetButton() const { return button; }
		inline bool IsModPressed(Mod mod) const { return mods & (int)mod; }

		std::string ToString() const override {
			std::stringstream string;
			string << "MouseButtonReleasedEvent: " << (int)button;
			return string.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryMouse | EventCategoryInput)

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	class MouseMovedEvent final : public Event {
	private:
		float mouseX, mouseY;

	public:
		MouseMovedEvent() = default;
		MouseMovedEvent(const float x, const float y)
			: mouseX(x), mouseY(y) { }

		inline float GetX() const { return mouseX; }
		inline float GetY() const { return mouseY; }

		std::string ToString() const override {
			std::stringstream string;
			string << "MouseMovedEvent X: " << mouseX << " Y: " << mouseY;
			return string.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

		EVENT_CLASS_TYPE(MouseMoved)
	};

	class MouseScrolledEvent final : public Event
	{
	private:
		float xOff;
		float yOff;

	public:
		MouseScrolledEvent() = default;
		MouseScrolledEvent(float xOff, float yOff)
			: xOff(xOff), yOff(yOff) { }

		inline float GetXOffset() const { return xOff; }
		inline float GetYOffset() const { return yOff; }

		std::string ToString() const override {
			std::stringstream string;
			string << "MouseScrolledEvent X-Offset: " << xOff << " Y-Offset: " << yOff;
			return string.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

		EVENT_CLASS_TYPE(MouseScrolled)
	};


}