#include "_Core.h"

#include "generic/Application.h"

#include "generic/Window.h"
#include "event/ApplicationEvent.h"

namespace core {

	Application* Application::instance;

	Application::Application() {
		instance = this;
		Window::setEventCallback(BIND_EVENT_FN(Application::onEvent));
	}

	Application::~Application() { }

	void Application::init() { }

	void Application::onEvent(Event& event)
	{
		LOG_CORE_TRACE("{0}", event);
	}

	void Application::changeScene(Scene* newScene)
	{
		Window::changeScene(newScene);
	}

	void Application::run() {
		Log::init();

		Window::run();
	}

	Application* Application::get() {
		return instance;
	}

}
