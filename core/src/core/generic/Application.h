#pragma once
#include "_Core.h"
#include "utility.h"

#include "generic/Window.h"
#include "generic/Scene.h"
#include "event/Event.h"

#include "layer/ImGuiLayer.h"

namespace core {

	class CORE_API Application
	{
	private:
		static Application* instance;
		Window* window = nullptr;
		ImGuiLayer* imguilayer = nullptr;
		Scene* current_scene = nullptr;
		Scene* queued_scene = nullptr;

		float dt;
		bool game_running = true;
	public:
		Application();
		virtual ~Application();

		virtual void init();

		void run();

		void onEvent(Event& event);
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);

		void changeScene(Scene* new_scene);

		void exit();

		static Application* get();
		static Window* getWindow() { return get()->window; }
		static Scene* getCurrentScene() { return get()->current_scene; }
	};
	
	//defined by client
	Application* createApplication();
}

