#pragma once
#include "Engine.h"
#include "utility.h"

#include "generic/Window.h"
#include "generic/Scene.h"
#include "layer/Layer.h"
#include "layer/LayerStack.h"

#include "event/ApplicationEvent.h"



namespace Paper {

	class ImGuiLayer;

	class Application
	{
	public:

		Application(const WindowProps& props = WindowProps());
		virtual ~Application();

		virtual void Init() = 0;
		virtual void BeforePollEvents() {}

		void Run();

		void OnEvent(Event& event);

		static void ChangeScene(Scene* new_scene);

		void Exit(bool restart = false);

		void RemoveAllLayers();

		void AddLayer(Layer* layer);
		void AddOverlay(Layer* layer);
		void RemoveLayer(Layer* layer);
		void RemoveOverlay(Layer* layer);

		static void SubmitToMainThread(const std::function<void()>& func);

		static Application* GetInstance() { return instance; }

		static float GetDT() { return GetInstance()->dt; }
		static bool IsResizing() { return GetInstance()->resizing; }
		static void SetEventCallback(const EventCallbackFunction& callbackFunction) { GetInstance()->window->SetEventCallback(callbackFunction); }

		static Shr<Window> GetWindow() { return GetInstance()->window; }
		static Scene* GetActiveScene() { return GetInstance()->currentScene; }
		static ImGuiLayer& GetImGuiLayer() { return *GetInstance()->imguiLayer; }
		static LayerStack& GetLayerStack() { return GetInstance()->layerStack; }

		inline static bool restart = false;

	protected:
		WindowProps windowProps;

	private:
		static Application* instance;
		Shr<Window> window = nullptr;
		ImGuiLayer* imguiLayer = nullptr;
		Scene* currentScene = nullptr;
		Scene* queuedScene = nullptr;
		float dt;
		bool gameRunning = true;
		bool resizing = false;

		std::vector<std::function<void()>> mainThreadQueue;
		std::mutex mainThreadQueueMutex;

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		void ExecuteMainThreadQueues();

		friend class Scene;

		LayerStack layerStack;
	};

	//defined by client
	Application* CreateApplication();
}

