#include "_Core.h"
#include "utility.h"

#include "generic/Window.h"
#include "event/ApplicationEvent.h"
#include "event/KeyEvent.h"
#include "event/MouseEvent.h"

#include <GLAD/glad.h>

namespace core {

    static void errorCallback(int error, const char* description)
    {
        CORE_ASSERT("GLFW ERROR {0}: {1}", error, description);
    }

    Window::Window(const WindowProps& window_props)
    {
        init(window_props);
    }

    Window::~Window()
    {
        quit();
    }

    void Window::init(const WindowProps& window_props)
    {
        window_data.title = window_props.title;
        window_data.width = window_props.width;
        window_data.height = window_props.height;

        LOG_CORE_TRACE("Creating window {0} ({1}, {2})", window_props.title, window_props.width, window_props.height);

        if (!initialized)
        {
	        int const success = glfwInit();
	        CORE_ASSERT(success, "Could not init GLFW.");
	        glfwSetErrorCallback(errorCallback);
            initialized = true;
        }

        glfwWindow = glfwCreateWindow(window_props.width, window_props.height, window_props.title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(glfwWindow);
        glfwSetWindowUserPointer(glfwWindow, &window_data);
        setVSync(true);

        int const success = gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));
        CORE_ASSERT(success, "Could not init glad!");


        //callbacks
        glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* window, int width, int height)
        {
        	WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.callback(event);
        });

        glfwSetWindowCloseCallback(glfwWindow, [](GLFWwindow* window)
        {
        	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.callback(event);
        });

        glfwSetKeyCallback(glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
        	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

	        switch (action)
	        {
	            case GLFW_PRESS:
                {
	            		KeyPressedEvent event(key, 0);
	            		data.callback(event);
	            		break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.callback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.callback(event);
                    break;
                }
                default:;
            }
        });

        glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* window, int button, int action, int mods)
        {
        	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.callback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.callback(event);
                    break;
                }
                default:;
            }
        });

        glfwSetScrollCallback(glfwWindow, [](GLFWwindow* window, double xOffset, double yOffset)
        {
        	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.callback(event);
        });

        glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* window, double xPos, double yPos)
        {
        	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        	MouseMovedEvent event((float)xPos, (float)yPos);
			data.callback(event);
        });
    }

    void Window::quit()
    {
        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }


    void Window::update(float dt)
    {
        glfwPollEvents();
        glfwSwapBuffers(glfwWindow);
    }

    void Window::setVSync(bool enabled)
    {
        glfwSwapInterval(enabled);
        window_data.vsync = enabled;
    }

    Window* Window::createWindow(const WindowProps& window_props)
    {
        return new Window(window_props);
    }

    


    int Window::run() {
        // declaration of standard values
        width = 960;
        height = 640;
        title = "BunkerGame";
        r = 1.0f;
        g = 1.0f;
        b = 1.0f;
        a = 1.0f;

        // functions that are mandatory to be called
        if (init() == -1) return -1;

        LOG_CORE_TRACE("Initialized and created window!");

        imGuiLayer = new ImGuiLayer();
        imGuiLayer->init();
        LOG_CORE_TRACE("Initialized ImGui!");

        loop();


        imGuiLayer->destroy();
        delete imGuiLayer;
        LOG_CORE_TRACE("Destroyed ImGui and exited!");

        glfwTerminate();

        return 0;
    }

    int Window::init() {
        // init library
        if (!glfwInit()) {
            LOG_CORE_ERROR("Could not initialize glfw library");
            return -1;
        }

        //Configure GLFW
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
        // create window
        glfwWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

        //self-explanatory
        if (!Window::glfwWindow) {
            LOG_CORE_ERROR("Could not init Window.");
            return -1;
        }
        // set listeners with callback functions (they run in separate threads)
        // keyboard pressing
        glfwSetKeyCallback(glfwWindow, KeyListener::keyCallback);
        // mouse button pressing
        glfwSetMouseButtonCallback(glfwWindow, MouseListener::mouseButtonCallback);
        // mouse scrolling
        glfwSetScrollCallback(glfwWindow, MouseListener::mouseScrollCallback);
        // mouse movement
        glfwSetCursorPosCallback(glfwWindow, MouseListener::mousePosCallback);

        glfwSetFramebufferSizeCallback(glfwWindow, Window::resizeCallback);
        //OpenGl context current
        glfwMakeContextCurrent(glfwWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CORE_ERROR("Failed to load glad");
            return -1;
        }

        //Enable v-sync
        glfwSwapInterval(1);

        //Window visible
        glfwShowWindow(glfwWindow);
        //enable blending with glad because imgui overrides the standard glEnabe
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        //APP SCENE LOGIC

        Application::get()->init();

        if (tempScene) {
            currentScene = tempScene;
            currentScene->initGeneral();
            tempScene = nullptr;
        }

        return 0;
    }

    // declaration of variables
    float beginTime;
    float endTime;
    float deltaTime;

    int Window::loop() {
        // start of the calculations
        beginTime = (float)glfwGetTime();
        deltaTime = -1.0f;
        float count = 0.0f;
        while (!glfwWindowShouldClose(glfwWindow)) {
            //Poll events
            glfwPollEvents();

            MouseListener::resetValues();
            if (currentScene != nullptr) {
                glm::vec4 sceneBackcolor = currentScene->getBackcolor();
                //clear color buffer
                glClearColor(sceneBackcolor.x, sceneBackcolor.y, sceneBackcolor.z, sceneBackcolor.w);
                glClear(GL_COLOR_BUFFER_BIT);

                if (deltaTime >= 0) {
                    if (tempScene != nullptr) {
                        // TODO: save scenes instead of deleting them
                        // delete the scene with it's heap components (renderer and camera)
                        currentScene->disable();

                        // remove the scene
                        delete currentScene;
                        // switch and initialize the scene
                        currentScene = tempScene;
                        currentScene->initGeneral();
                        // don't forget to reset the tempscene, because we want to override it
                        tempScene = nullptr;
                    }
                    currentScene->update(deltaTime);

                    Window::getImGuiLayer()->update(deltaTime, currentScene);
                }
            }
            else if (warn) {
                LOG_CORE_ERROR("No Scene exists. Make sure to call Application::changeScene() in the 'init' function of your Application class");
                warn = false;
            }

            // switch front buffer with back buffer
            glfwSwapBuffers(glfwWindow);
            glViewport(0, 0, width, height);

            //FPS IN TITLE
            count -= deltaTime;
            if (count <= 0) {
                int fps = 1 / deltaTime;
                char buffer[10];
                //sprintf(buffer, "%d", fps);
                sprintf_s(buffer, 10, "%d", fps);
                Window::setTitle("BunkerGame | FPS: " + std::string(buffer));
                count = 1.0f;
            }
            // dt calculations
            // get passed time, subtract it to get the current frame time, then start with endTime (most updated time)
            // keep in mind, 1 frame will be lost for calculation
            endTime = (float)glfwGetTime();
            deltaTime = endTime - beginTime;
            beginTime = endTime;
        }
        return 0;
    }

    void Window::quit(std::string message) {
        LOG_CORE_WARN("Quitting");
        if (message != "") {
            LOG_CORE_ERROR(message);
        }
        //close glfwWindow
        glfwSetWindowShouldClose(Window::getGLFWwindow(), 88);
    }

    void Window::changeScene(Scene* newScene) {
        // using tempScene enables us the functionality to let the current scene finish rendering for the current frame
        // the next frame will use the new scene
        tempScene = newScene;
    }

    void Window::resizeCallback(GLFWwindow* window, int width, int height)
    {
        Window::width = width;
        Window::height = height;
        glfwSetWindowSize(window, width, height);
    }

    void Window::setTitle(std::string title) {
        Window::title = title;
        glfwSetWindowTitle(Window::glfwWindow, title.c_str());
    }

    GLFWwindow* Window::getGLFWwindow() {
        return Window::glfwWindow;
    }

    std::string Window::getTitle() {
        return Window::title;
    }

    Scene* Window::getScene() {
        return currentScene;
    }

    float Window::getDeltaTime() {
        return deltaTime;
    }

    ImGuiLayer* Window::getImGuiLayer() {
        return imGuiLayer;
    }

}