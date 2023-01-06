#pragma once

#include "utility.h"

#include "generic/Scene.h"

namespace core {

	class CORE_API Application
	{
	private:
		static Application* instance;
	public:
		Application();
		virtual ~Application();

		virtual void init();

		void changeScene(Scene* newScene);
		void run();


		static Application* get();
	};
	
	//defined by client
	Application* createApplication();
}

