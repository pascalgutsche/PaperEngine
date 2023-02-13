#pragma once
#include "_Core.h"
#include "utility.h"

#include "event/Event.h"
//WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
//AppTick, AppUpdate, AppRender,

namespace core {
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		inline Event& clone() const override
		{
			WindowCloseEvent event(*this);
			return event;
		};

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowClose)
	};

	class WindowResizeEvent : public Event
	{
	private:
		unsigned int width, height;

	public:
		WindowResizeEvent() = default;
		WindowResizeEvent(const unsigned int width, const unsigned int height)
			: width(width), height(height) { }

		inline unsigned int getWidth() const { return width; }
		inline unsigned int getHeight() const { return height; }

		inline Event& clone() const override
		{
			WindowResizeEvent event(*this);
			return event;
		};

		std::string toString() const override
		{
			std::stringstream string;
			string << "WindowResizeEvent width: " << width << " height: " << height;
			return string.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowResize)
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		inline Event& clone() const override
		{
			AppTickEvent event(*this);
			return event;
		};

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		inline Event& clone() const override
		{
			AppUpdateEvent event(*this);
			return event;
		};

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		inline Event& clone() const override
		{
			AppRenderEvent event(*this);
			return event;
		};

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}