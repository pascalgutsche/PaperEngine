#pragma once
#include "Engine.h"
#include "utility.h"

#include "event/Event.h"
#include "ButtonCodes.h"

namespace Paper {

	class KeyPressedEvent : public Event {
	private:
		bool repeated;
		Key code;
		int mods;
	public: 
		KeyPressedEvent(Key code, const int repeat_count, const int mods)
			: repeated(repeat_count > 0), code(code), mods(mods) { }

		inline Key GetKeyCode() const { return code; }
		inline bool GetRepeated() const { return repeated; }
		inline bool IsModPressed(Mod mod) const { return mods & (int)mod; }

		std::string ToString() const override {
			std::stringstream string;
			string << "KeyPressedEvent: " << (int)code << " | Repeated: " << repeated;
			return string.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryKeyBoard | EventCategoryInput)

		EVENT_CLASS_TYPE(KeyPressed)
	};

	class KeyReleasedEvent : public Event {
	private:
		Key code;
		int mods;
	public:
		KeyReleasedEvent(Key code, const int mods)
			: code(code), mods(mods) { }

		inline Key GetKeyCode() const { return code; }
		inline bool IsModPressed(Mod mod) const { return mods & (int)mod; }

		std::string ToString() const override {
			std::stringstream string;
			string << "KeyReleasedEvent: " << (int)code;
			return string.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryKeyBoard | EventCategoryInput)

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public Event {
	private:
		Key code;
	public:
		KeyTypedEvent() = default;
		KeyTypedEvent(Key code)
			: code(code) { }

		inline Key getKeyCode() const { return code; }

		std::string ToString() const override {
			std::stringstream string;
			string << "KeyPressedEvent: " << (int)code;
			return string.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryKeyBoard | EventCategoryInput)

		EVENT_CLASS_TYPE(KeyTyped)
	};

	
}

