#include "_Core.h"

#include "generic/Application.h"

#include "event/Input.h"
#include "event/KeyCodes.h"
#include "renderer/RenderCommand.h"
#include "renderer/Renderer.h"
#include "imgui/ImGuiLayer.h"
#include "utils/Core.h"


namespace core {

	Application* Application::instance;

	Application::Application() {
		Log::init();
		CORE_ASSERT(!instance, "application is already instanced!");
		instance = this;

		Core::Init();

		window = Window::Create();
		SetEventCallback(BIND_EVENT_FN(Application::onEvent));

		Renderer::Init();

		

		imguilayer = new ImGuiLayer();
	}

	Application::~Application()
	{
		exit();
	}

	void Application::init() { }

	void Application::QueueEvents(Event* event)
	{
		GetInstance()->eventQueue.emplace(GetInstance()->eventQueue.begin(), event);
	}


	void Application::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::onWindowResize));
		dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::onKeyPressed));

		for (auto it = layer_stack.end(); it != layer_stack.begin(); )
		{
			if (event.handled)
				break;
			(*--it)->event(event);
		}
		if (!event.handled)
		{
			currentScene->OnEvent(event);
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		game_running = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		resizing = true;
		Renderer::ResizeWindow(e.getWidth(), e.getHeight());
		return false;
	}


	bool Application::onKeyPressed(KeyPressedEvent& e)
	{
		if (!e.getRepeated() && e.getKeyCode() == KEY_P)
		{
			if (imgui_enabled_queue == 0 && imgui_enabled) imgui_enabled_queue = 1;
			else imgui_enabled_queue = 2;
			return true;
		}
		return false;
	}

	void Application::ProcessQueues()
	{
		if (imgui_enabled_queue > 0)
		{
			imgui_enabled = imgui_enabled_queue - 1;
			imgui_enabled_queue = 0;
		}

		for (Event* event : eventQueue)
		{
			onEvent(*event);
			delete event;
		}
		eventQueue.clear();
	}

	void Application::run() 
	{
		init();

		AddOverLay(imguilayer, false);


		//set start scene
		if (queuedScene) {
			currentScene = queuedScene;
			currentScene->InitGeneral();
			queuedScene = nullptr;
		}

		// start of the calculations
		float begin_time = window->GetTime();
		dt = 0.0167f;
		bool warn = true;

		while (game_running)
		{
			window->PollEvents();
			ProcessQueues();

			//MouseListener::resetValues();
			if (currentScene != nullptr) {
				// request color
				RenderCommand::ClearColor(currentScene->GetBackcolor());
				

				if (dt >= 0) {
					if (queuedScene != nullptr) {
						// TODO: save scenes instead of deleting them
						// delete the scene with it's heap components (renderer and camera)
						currentScene->Disable();

						// remove the scene
						delete currentScene;
						// switch and initialize the scene
						currentScene = queuedScene;
						currentScene->InitGeneral();
						// don't forget to reset the tempscene, because we want to override it
						queuedScene = nullptr;
					}

					imguilayer->begin(dt);

					for (Layer* layer : layer_stack)
						layer->update(dt);

					currentScene->OnUpdate();

					Input::ProcessInput();
					
					if (imgui_enabled) {
					
						for (Layer* layer : layer_stack) {
							layer->imgui(dt);
						}
					}
					else
					{
						imguilayer->ScreenPanel();
					}

					imguilayer->end();
				}
			}
			else if (warn) {
				LOG_CORE_ERROR("No Scene exists. Make sure to call Application::changeScene() in the 'init' function of your Application class");
				warn = false;
			}

			window->SwapBuffers();

			imguiEnabledBefore = imgui_enabled;
			resizing = false;

			dt = window->GetTime() - begin_time;
			begin_time = window->GetTime();
		}

		LOG_CORE_WARN("Reached end of game function. Shutting down.");
	}

	void Application::ChangeScene(Scene* new_scene)
	{
		GetInstance()->queuedScene = new_scene;
	}

	void Application::AddLayer(Layer* layer, bool add_to_renderer)
	{
		GetInstance()->layer_stack.addLayer(layer);
		layer->attach(add_to_renderer);
	}

	void Application::AddOverLay(Layer* layer, bool add_to_renderer)
	{
		GetInstance()->layer_stack.addOverlay(layer);
		layer->attach(add_to_renderer);
	}

	void Application::RemoveLayer(Layer* layer)
	{
		layer->detach();
		GetInstance()->layer_stack.removeLayer(layer);
	}

	void Application::RemoveOverLay(Layer* layer)
	{
		layer->detach();
		GetInstance()->layer_stack.removeOverlay(layer);
	}
}
