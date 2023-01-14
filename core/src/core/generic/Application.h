#pragma once
#include "_Core.h"
#include "utility.h"

#include "generic/Window.h"
#include "generic/Scene.h"
#include "layer/Layer.h"
#include "layer/LayerStack.h"

#include "event/ApplicationEvent.h"
#include "event/KeyEvent.h"

#include "ImGui/ImGuiLayer.h"

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
		long int frames_rendered = 0;
		bool game_running = true;
		bool imgui_enabled = true; int imgui_enabled_queue = 0;

		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);
		bool onKeyPressed(KeyPressedEvent& e);

		void ProcessQueues();

		LayerStack layer_stack;

	public:
		Application();
		virtual ~Application();

		virtual void init();
		
		void run();

		void onEvent(Event& event);

		void addLayer(Layer* layer);
		void addOverLay(Layer* layer);

		void changeScene(Scene* new_scene);

		void exit();

		static long int GetFramesRendered() { return get()->frames_rendered; }
		static bool GetImGuiEnabled() { return get()->imgui_enabled; }

		static Application* get();
		static Window* getWindow() { return get()->window; }
		static Scene* getCurrentScene() { return get()->current_scene; }
		static ImGuiLayer& IMGUI() { return *get()->imguilayer; }
	};
	
	//defined by client
	Application* createApplication();
}

