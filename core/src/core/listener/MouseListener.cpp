#include "_Core.h"
#include <GLFW/glfw3.h>

#include "listener/MouseListener.h"
#include "generic/Window.h"

namespace core {

    const int length = 15;

    static double dxHistory[length];
    static double dyHistory[length];
    static double scrollXHistory[length];
    static double scrollYHistory[length];

    const int mouseButtonLength = 3;

    static double scrollX = 0.0;
    static double scrollY = 0.0;
    static double xPos = 0.0;
    static double yPos = 0.0;
    static double lastX = 0.0;
    static double lastY = 0.0;
    static bool mouseButtonPressed[mouseButtonLength];
    static bool dragging = false;

    static double dx = 0.0;
    static double dy = 0.0;

    void MouseListener::mousePosCallback(GLFWwindow* glfwWindow, double xpos, double ypos) {
        // just set callback variables
        lastX = xPos;
        lastY = yPos;
        xPos = xpos;
        yPos = ypos;
        dragging = mouseButtonPressed[0] || mouseButtonPressed[1] || mouseButtonPressed[2];
    }

    void MouseListener::mouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods) {
        // input handling, array security, do not go over the amount of buttons one has
        if (action == GLFW_PRESS) {
            if (button < mouseButtonLength) {
                mouseButtonPressed[button] = true;
            }
        }
        else if (action == GLFW_RELEASE) {
            if (button < mouseButtonLength) {
                mouseButtonPressed[button] = false;
            }
        }
    }

    void MouseListener::mouseScrollCallback(GLFWwindow* glfwWindow, double xOffset, double yOffset) {
        scrollX = xOffset;
        scrollY = yOffset;
    }

    void MouseListener::resetValues() {
        dx = lastX - xPos;
        dy = lastY - yPos;

        // check for all inputs
        manageValues(dxHistory, 0);
        manageValues(dyHistory, 1);
        manageValues(scrollXHistory, 2);
        manageValues(scrollYHistory, 3);
    }

    int tmp[4] = { 0, 0, 0, 0 };

    void MouseListener::manageValues(double array[], int val) {
        double placeholder;
        // give placeholder the value of dx ||dy
        // in order to check if the same number occurs multiple times (for more, see below)

        // value reference list:
        // | [0]: dx | [1]: dy | [2]: scrollX | [3]: scrollY |
        if (val == 0) {
            placeholder = dx;
        }
        if (val == 1) {
            placeholder = dy;
        }
        if (val == 2) {
            placeholder = scrollX;
        }
        if (val == 3) {
            placeholder = scrollY;
        }

        // re-assign the first 10 values in the array
        array[tmp[val]] = placeholder;

        // if it gets up to length value (array maximum) start all over again from 0
        tmp[val]++;
        // if the array has been filled reset the counter
        if (tmp[val] == length) tmp[val] = 0;

        // temp has the comparison value (if every value is the same, you could take any value in any index)
        double temp = array[0];
        int count = 0;
        // check for same numbers
        for (int i = 0; i < length; i++)
        {
            if (temp == array[i])
            {
                count++;
            }
        }
        // if every number is the same set placeholder (dx or dy) to 0
        // (which fixes the bug where the distance (dx ||dy) always had a value,
        // even though one did not move the cursor)
        if (count == length)
        {
            placeholder = 0;
        }

        // decide what distance to reset based upon the function parameter: val
        if (val == 0) {
            dx = placeholder;
        }
        if (val == 1) {
            dy = placeholder;
        }
        if (val == 2) {
            scrollX = placeholder;
        }
        if (val == 3) {
            scrollY = placeholder;
        }
    }

    double MouseListener::getX() {
        return xPos;
    }

    double MouseListener::getOrthoX() {
        /*
        float currentX = getX();
        currentX = (currentX / (float)Window::getWindow()->width) * 2 - 1.0f;
        glm::vec4 temp = glm::vec4(currentX, 0, 0, 1);
        temp = (temp * Window::getWindow()->getScene()->getCamera().getReverseProjectionMatrix()) * Window::getWindow()->getScene()->getCamera().getReverseViewMatrix();
        currentX = temp.x;
        //Logger::Log(std::to_string(currentX));
    */
        return 0;
    }

    double MouseListener::getY() {
        return yPos;
    }

    double MouseListener::getOrthoY() {
        return 0;
    }

    double MouseListener::getDx() {
        return dx;
    }

    double MouseListener::getDy() {
        return dy;
    }

    double MouseListener::getScrollX() {
        return scrollX;
    }

    double MouseListener::getScrollY() {
        return scrollY;
    }

    bool MouseListener::isDragging() {
        return dragging;
    }

    bool MouseListener::mouseButtonDown(int button) {
        if (button < mouseButtonLength) {
            return mouseButtonPressed[button];
        }
        return false;
    }

}