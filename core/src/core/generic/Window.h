#pragma once
#include "_Core.h"
#include "utility.h"

#include "generic/Scene.h"
#include "renderer/ImGuiLayer.h"


#include <GLFW/glfw3.h>

namespace core {

    class Window {
    private:
        static std::string title;
        static GLFWwindow* glfwWindow;
        static Scene* currentScene;
        static Scene* tempScene;

        static ImGuiLayer* imGuiLayer;

        // initializations and native loop
        static int init();
        static int loop();

        // resize window callback
        static void resizeCallback(GLFWwindow* window, int width, int height);

    public:
        static int width, height;
        static float r;
        static float g;
        static float b;
        static float a;

        static int run();
        static void quit(std::string message = "");
        static void changeScene(Scene* newScene);

        static float getDeltaTime();
        static Scene* getScene();
        static GLFWwindow* getGLFWwindow();
        static std::string getTitle();
        static ImGuiLayer* getImGuiLayer();

        static void setTitle(std::string title);

        friend int main(int argc, char* argv[]);
    };
}