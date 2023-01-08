#pragma once
#include "_Core.h"
#include "utility.h"

#include "event/Event.h"

namespace core {

	class KeyPressedEvent : public Event {
	private:
		bool repeated;
		int keyCode;
	public: 
		KeyPressedEvent(int keyCode, int repeatCount)
			: keyCode(keyCode), repeated(repeatCount > 0) { }

		inline int getKeyCode() const { return keyCode; }
		inline bool getRepeated() const { return repeated; }

		std::string toString() const override {
			std::stringstream string;
			string << "KeyPressedEvent: " << keyCode << " | Repeated: " << repeated;
			return string.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryKeyBoard | EventCategoryInput)

		EVENT_CLASS_TYPE(KeyPressed)
	};

	class KeyReleasedEvent : public Event {
	private:
		int keyCode;
	public:
		KeyReleasedEvent(int keyCode)
			: keyCode(keyCode) { }

		inline int getKeyCode() const { return keyCode; }

		std::string toString() const override {
			std::stringstream string;
			string << "KeyReleasedEvent: " << keyCode;
			return string.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryKeyBoard | EventCategoryInput)

		EVENT_CLASS_TYPE(KeyReleased)
	};

	
}

