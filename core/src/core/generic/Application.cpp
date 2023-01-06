#include "_Core.h"

#include "generic/Application.h"

#include "generic/Window.h"

namespace core {

	Application* Application::instance;

	Application::Application() {
		instance = this;
	}

	Application::~Application() { }

	void Application::init() { }

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
