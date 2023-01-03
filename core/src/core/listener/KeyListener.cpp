#include "_Core.h"
#include <GLFW/glfw3.h>

#include "listener/KeyListener.h"

// static definition is needed for using it without an instance
// they may not be utilized in declarations (.cpp files)
static bool keyPressed[350];

// this function sets
void KeyListener::keyCallback(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods) {
    // self-explanatory || key = glfw enums
    if (action == GLFW_PRESS) {
        keyPressed[key] = true;
    } else if (action == GLFW_RELEASE) {
        keyPressed[key] = false;
    }
}

// this function gives
bool KeyListener::isKeyPressed(int keyCode) {
    return keyPressed[keyCode];
}