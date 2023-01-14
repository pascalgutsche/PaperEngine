#include "_Core.h"

#include "generic/Application.h"
#include "event/KeyCodes.h"
#include "event/Input.h"

#include "glad/glad.h"

namespace core {

	Application* Application::instance;

	Application::Application() {
		instance = this;
		Log::init();

		window = Window::createWindow();
		window->setVSync(false);
		setEventCallback(BIND_EVENT_FN(Application::onEvent));

		imguilayer = new ImGuiLayer();
	}

	Application::~Application()
	{
		exit();
	}

	void Application::init() { }

	void Application::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::onWindowResize));
		dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::onKeyPressed));


		for (auto it = layer_stack.end(); it != layer_stack.begin(); )
		{
			(*--it)->event(event);
			if (event.handled)
				break;
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		game_running = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
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
		if (!e.getRepeated() && e.getKeyCode() == KEY_L)
		{
			this->IMGUI().DockPanel("Application: ", IMGUI().getDockspaceRIGHT());
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
	}

	void Application::run() 
	{

		init();

		addLayer(imguilayer);

		//set start scene
		if (queued_scene) {
			current_scene = queued_scene;
			current_scene->initGeneral();
			queued_scene = nullptr;
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
			if (current_scene != nullptr) {
				const glm::vec4 scene_backcolor = current_scene->getBackcolor();
				//clear color buffer
				glClearColor(scene_backcolor.x, scene_backcolor.y, scene_backcolor.z, scene_backcolor.w);
				glClear(GL_COLOR_BUFFER_BIT);

				if (dt >= 0) {
					if (queued_scene != nullptr) {
						// TODO: save scenes instead of deleting them
						// delete the scene with it's heap components (renderer and camera)
						current_scene->disable();

						// remove the scene
						delete current_scene;
						// switch and initialize the scene
						current_scene = queued_scene;
						current_scene->initGeneral();
						// don't forget to reset the tempscene, because we want to override it
						queued_scene = nullptr;
					}

					current_scene->update(dt);

					for (Layer* layer : layer_stack)
						layer->update(dt);

					if (imgui_enabled) {

						imguilayer->begin(dt);
						for (Layer* layer : layer_stack)
							layer->imgui(dt);
						imguilayer->end();
					}
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

	void Application::changeScene(Scene* new_scene)
	{
		queued_scene = new_scene;
	}

	void Application::addLayer(Layer* layer)
	{
		layer_stack.addLayer(layer);
		layer->attach();
	}

	void Application::addOverLay(Layer* layer)
	{
		layer_stack.addOverlay(layer);
		layer->attach();
	}

}
