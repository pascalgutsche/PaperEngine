#pragma once
#include "_Core.h"
#include "utility.h"

#include "generic/Window.h"
#include "generic/Scene.h"
#include "event/Event.h"

namespace core {

	class Application
	{
	private:
		static Application* instance;
		Window* window = nullptr;
	public:
		virtual void init();

		void run();

		void onEvent(Event& event);
		void changeScene(Scene* new_scene);

		void exit();

		static Application* get();
	};
	
	//defined by client
	Application* createApplication();
}

