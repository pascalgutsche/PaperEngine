#include "_Core.h"

#include "generic/Application.h"
#include "event/KeyCodes.h"
#include "event/Input.h"

#include "utils/Core.h"


namespace core {

	Application* Application::instance;

	Application::Application() {
		instance = this;
		Log::init();

		window = Window::createWindow();
		SetEventCallback(BIND_EVENT_FN(Application::onEvent));

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

		Core::Init();

		//set start scene
		if (queuedScene) {
			currentScene = queuedScene;
			currentScene->InitGeneral();
			queuedScene = nullptr;
		}

		// start of the calculations
		float begin_time = static_cast<float>(glfwGetTime());
		dt = 0.0167f;
		bool warn = true;

		while (game_running)
		{
			glfwPollEvents();

			ProcessQueues();

			//MouseListener::resetValues();
			if (currentScene != nullptr) {
				// request color
				RenderCommand::ClearColor(currentScene->GetBackcolor());
				// set color
				RenderCommand::Clear();

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

					if (imgui_enabled) {

						for (Layer* layer : layer_stack) {
							layer->imgui(dt);
						}
					}

					for (Layer* layer : layer_stack)
						layer->update(dt);

					currentScene->update(dt);

					imguilayer->end();
					
					frames_rendered++;

				}
			}
			else if (warn) {
				LOG_CORE_ERROR("No Scene exists. Make sure to call Application::changeScene() in the 'init' function of your Application class");
				warn = false;
			}

			glfwSwapBuffers(window->getNativeWindow());

			dt = static_cast<float>(glfwGetTime()) - begin_time;
			begin_time = static_cast<float>(glfwGetTime());
		}

		LOG_CORE_WARN("Reached end of game function. Shutting down.");
		delete window;
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
