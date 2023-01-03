#include "_Core.h"
#include <GLFW/glfw3.h>

#include "generic/Window.h"
#include "generic/Scene.h"
#include "listener/KeyListener.h"
#include "listener/MouseListener.h"

Scene* Window::currentScene;
Scene* Window::tempScene;
GLFWwindow* Window::glfwWindow;
std::string Window::title;
std::unordered_map<std::string, Scene*> Scene::sceneMap;
float Window::r;
float Window::g;
float Window::b;
float Window::a;
ImGuiLayer* Window::imGuiLayer;
static bool warn = true;

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
    if (init() == -1) {
        return -1;
    }
    Logger::Log("Initialized and created window!", Logger::Trace);

    imGuiLayer = new ImGuiLayer();
    imGuiLayer->init();
    Logger::Log("Initialized ImGui!", Logger::Trace);

    loop();

    imGuiLayer->destroy();
    delete imGuiLayer;
    Logger::Log("Destroyed ImGui and exited!", Logger::Trace);

    glfwTerminate();

    return 0;
}

int Window::init() {
    // init library
    if (!glfwInit()) {
        Logger::Log("Could not initialize glfw library", Logger::Error);
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
        Logger::Log("Could not init Window.", Logger::Error);
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
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Logger::Log("Failed to load glad", Logger::Error);
        return -1;
    }

    //Enable v-sync
    glfwSwapInterval(1);

    //Window visible
    glfwShowWindow(glfwWindow);
    //enable blending with glad because imgui overrides the standard glEnabe
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

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
    while(!glfwWindowShouldClose(glfwWindow)) {
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
            }

            if (Logger::logLevel == Logger::Debug) {
                Window::getImGuiLayer()->update(deltaTime, currentScene);
            }
        }
        else if (Scene::sceneMap.find("initScene") != Scene::sceneMap.end()) {
            currentScene = Scene::sceneMap.at("initScene");
            currentScene->initGeneral();
        }
        else if (warn) {
            Logger::Log("Couldn't find any initScene inside the SceneMap", Logger::Level::Error);
            warn = false;
        }
        
        // switch front buffer with back buffer
        glfwSwapBuffers(glfwWindow);
        glViewport(0, 0, width, height);

        //FPS IN TITLE
        count -= deltaTime;
        if (count <= 0) {
            int fps = 1/deltaTime;
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
    Logger::Log("Quitting", Logger::Level::Warn);
    if (message != "") {
        Logger::Log(message, Logger::Error);
    }
    //close glfwWindow
    glfwSetWindowShouldClose(Window::getGLFWwindow(), 88);
}

void Window::changeScene(Scene* newScene) {
    // using tempScene enables us the functionality to let the current scene finish rendering for the current frame
    // the next frame will use the new scene
    tempScene = newScene;
}

void Window::resizeCallback(GLFWwindow *window, int width, int height)
{
    Window::width = width;
    Window::height = height;
    glfwSetWindowSize(window, width, height);
}

void Window::setTitle(std::string title) {
    Window::title = title;
    glfwSetWindowTitle(Window::glfwWindow, title.c_str());
}

GLFWwindow *Window::getGLFWwindow() {
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
