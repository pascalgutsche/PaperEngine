#pragma once
#include "_Core.h"
#include "utility.h"

namespace core {

    class CORE_API KeyListener {
    public:
        // function describers (like static) should be placed in the header if instance-less utilization is desired
        static void keyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);
        
        static bool isKeyPressed(int keyCode);
    };

}