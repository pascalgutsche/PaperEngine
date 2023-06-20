#pragma once
#include "Engine.h"
#include "utility.h"

#include "generic/Window.h"
#include "generic/Scene.h"
#include "layer/Layer.h"
#include "layer/LayerStack.h"

#include "event/ApplicationEvent.h"



namespace engine {

	class ImGuiLayer;

	class Application
	{
	public:

		Application(const WindowProps& props = WindowProps());
		virtual ~Application();

		virtual void Init();
		
		void Run();

		void OnEvent(Event& event);

		static void ChangeScene(Scene* new_scene);

		void Exit() { gameRunning = false; }

		void AddLayer(Layer* layer);
		void AddOverlay(Layer* layer);
		void RemoveLayer(Layer* layer);
		void RemoveOverlay(Layer* layer);

		static void QueueEvents(Event* event);
		static Application* GetInstance() { return instance; }

		static float GetDT() { return GetInstance()->dt; }
		static bool IsResizing() { return GetInstance()->resizing; }
		static void SetEventCallback(const EventCallbackFunction& callbackFunction) { GetInstance()->window->SetEventCallback(callbackFunction); }

		static Shr<Window> GetWindow() { return GetInstance()->window; }
		static Scene* GetActiveScene() { return GetInstance()->currentScene; }
		static ImGuiLayer& GetImGuiLayer() { return *GetInstance()->imguiLayer; }
		static LayerStack& GetLayerStack() { return GetInstance()->layerStack; }

	protected:
		WindowProps windowProps;

	private:
		static Application* instance;
		Shr<Window> window = nullptr;
		ImGuiLayer* imguiLayer = nullptr;
		Scene* currentScene = nullptr;
		Scene* queuedScene = nullptr;
		std::vector<Event*> eventQueue;
		float dt;
		bool gameRunning = true;
		bool resizing = false;

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		void ProcessQueues();

		friend class Scene;

		LayerStack layerStack;
	};

	//defined by client
	Application* CreateApplication();
}

