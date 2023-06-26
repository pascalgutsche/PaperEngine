#include "Engine.h"

#include "generic/Application.h"

#include "event/Input.h"
#include "event/KeyCodes.h"
#include "event/GameEvent.h"
#include "renderer/RenderCommand.h"
#include "renderer/Renderer2D.h"
#include "renderer/Renderer3D.h"
#include "imgui/ImGuiLayer.h"
#include "utils/Core.h"


namespace ppr {

	Application* Application::instance;

	Application::Application(const WindowProps& props)
	{
		Log::Init();
		CORE_ASSERT(!instance, "application is already instanced!");
		instance = this;

		Core::Init();

		window = Window::Create(props);
		SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		RenderCommand::Init();

		imguiLayer = new ImGuiLayer();
	}

	Application::~Application()
	{
		Exit();
	}

	void Application::Init() { }

	void Application::QueueEvents(Event* event)
	{
		GetInstance()->eventQueue.emplace(GetInstance()->eventQueue.begin(), event);
	}


	void Application::OnEvent(Event& event)
	{
		if (starting) return;
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
		dispatcher.dispatch<MouseMovedEvent>([this](MouseMovedEvent& e)
			{
				//this->GetActiveScene()->GetCamera()->MouseMoved(e);
				return false;
			});

		for (auto it = layerStack.end(); it != layerStack.begin(); )
		{
			if (event.handled)
				break;
			(*--it)->OnEvent(event);
		}
		//if (!event.handled)
		//{
		//	currentScene->OnEvent(event);
		//}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		gameRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		resizing = true;
		RenderCommand::SetViewPort(0, 0, e.getWidth(), e.getHeight());
		return false;
	}

	void Application::ProcessQueues()
	{

		for (Event* event : eventQueue)
		{
			OnEvent(*event);
			delete event;
		}
		eventQueue.clear();
	}

	void Application::Run() 
	{
		Init();

		AddOverlay(imguiLayer);


		//set start scene
		if (queuedScene) {
			currentScene = queuedScene;
			currentScene->Start();
			queuedScene = nullptr;
		}

		// start of the calculations
		float begin_time = window->GetTime();
		dt = 0.0167f;
		bool warn = true;

		while (gameRunning)
		{
			window->PollEvents();
			ProcessQueues();

			RenderCommand::ClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			RenderCommand::Clear();

			
			imguiLayer->Begin(dt);

			for (Layer* layer : layerStack)
			{
				if (layer->IsAttached())
					layer->Update(dt);
			}

			for (Layer* layer : layerStack) {
				layer->Imgui(dt);
			}

			Input::ProcessInput();

			imguiLayer->End();


			//if (currentScene != nullptr) {
			//	if (dt >= 0) {
			//		if (queuedScene != nullptr) {
			//			// TODO: save scenes instead of deleting them
			//			// delete the scene with it's heap components (renderer and camera)
			//			currentScene->Stop();
			//
			//			// remove the scene
			//			//delete currentScene;
			//			// switch and initialize the scene
			//			currentScene = queuedScene;
			//			currentScene->Start();
			//			// don't forget to reset the tempscene, because we want to override it
			//			queuedScene = nullptr;
			//		}
			//		currentScene->Update();
			//	}
			//}
			//else if (warn) {
			//	LOG_CORE_ERROR("No Scene exists. Make sure to call Application::changeScene() in the 'init' function of your Application class");
			//	warn = false;
			//}

			window->SwapBuffers();

			resizing = false;
			starting = false;

			dt = window->GetTime() - begin_time;
			begin_time = window->GetTime();
		}

		LOG_CORE_WARN("Reached end of game function. Shutting down.");
	}

	void Application::ChangeScene(Scene* new_scene)
	{
		GetInstance()->queuedScene = new_scene;
	}

	void Application::AddLayer(Layer* layer)
	{
		layerStack.AddLayer(layer);
		layer->Attach();
	}

	void Application::AddOverlay(Layer* layer)
	{
		layerStack.AddOverlay(layer);
		layer->Attach();
	}

	void Application::RemoveLayer(Layer* layer)
	{
		layer->Detach();
		layerStack.RemoveLayer(layer);
	}

	void Application::RemoveOverlay(Layer* layer)
	{
		layer->Detach();
		layerStack.RemoveOverlay(layer);
	}
}
