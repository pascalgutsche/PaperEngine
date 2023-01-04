#pragma once
#include "_Core.h"

namespace core {

    class MouseListener {
    private:
        static void manageValues(double array[], int val);

    public:
        // listener functions, used by glfwCallback(..) (they have their own thread
        static void mousePosCallback(GLFWwindow* glfwWindow, double xpos, double ypos);
        static void mouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods);
        static void mouseScrollCallback(GLFWwindow* glfwWindow, double xOffset, double yOffset);

        static void resetValues();

        // get values (for calculating mouse movements) / analyzing mouse actions
        static double getX();
        static double getOrthoX();
        static double getY();
        static double getOrthoY();
        static double getDx();
        static double getDy();
        static double getScrollX();
        static double getScrollY();

        // returns true if a mouse button is being dragged
        static bool isDragging();

        // this function reports if a mouse button has been pressed
        static bool mouseButtonDown(int button);
    };

}