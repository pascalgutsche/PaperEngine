#include "_Core.h"

#include "generic/Camera.h"
#include "generic/Application.h"

namespace core {

    Camera::Camera() {
        // standard position of the camera
        position = glm::vec3(0.0f, 0.0f, 5.0f);
        // standard fov
        fov = 1.0f;

        pitch = 0.0f;
        yaw = -90.0f;
        roll = 0.0f;

        lastX = Application::GetWindow()->GetWidth() / 2;
        lastY = Application::GetWindow()->GetHeight() / 2;

        front = glm::vec3(0.0f, 0.0f, -1.0f);
    }

    Camera::~Camera() {}


    void Camera::CalcCameraVectors() {
        //set vectors
        // the camera has to look somewhere (just set it to 0 because we are making it absolute)
        this->target = glm::vec3(this->position.x, this->position.y, 0.0f);

        direction.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
        direction.y = glm::sin(glm::radians(pitch));
        direction.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

        directionBruh.x = glm::cos(glm::radians(yaw));
        directionBruh.y = glm::sin(glm::radians(pitch));
        directionBruh.z = glm::sin(glm::radians(yaw));

        front = glm::normalize(direction);

        // this is being used for pointing upwards (y globally, e.g. in the world)
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        // declare z-axis
        cameraDirection = glm::normalize(position - target);
        // declare x-axis by the cross-product
        cameraRight = glm::normalize(glm::cross(up, cameraDirection));
        // declare y-axis by the cross-product of z and x
        cameraUp = glm::cross(cameraDirection, cameraRight);
        // make camera depend on position (who could have thought this)
        cameraViewMatrix = glm::lookAt(position, position + front, up);

        
    }

    glm::mat4 Camera::GetViewMatrix() {
        // give camera an "eye"
        return cameraViewMatrix;
    }

    glm::mat4 Camera::GetReverseViewMatrix() {
        return glm::inverse(Camera::GetViewMatrix());
    }

    glm::mat4 Camera::GetProjectionMatrix() {
        // get aspect ratio because we need it in order to project the camera correctly
        float aspect = (float)Application::GetWindow()->GetWidth() / (float)Application::GetWindow()->GetHeight();
        return glm::perspective(fov, aspect, 0.1f, 1000.0f);
    }

    glm::mat4 Camera::GetOrthographicMatrix()
    {
        // ortograhpic camera is basically a custom coordinate system with one's own units
        // every gameobject has the option to be rendered relative to orthographic or to projection
        return glm::ortho(0.0f, 2.0f, 0.0f, 2.0f, 0.0f, 10000000.0f);
    }

    glm::mat4 Camera::GetReverseProjectionMatrix() {
        return glm::inverse(Camera::GetProjectionMatrix());
    }

    glm::vec3 Camera::GetFront() const
    {
        return glm::normalize(directionBruh);;
    }

    //TODO: MOVE THIS SOMEWHERE ELSE
    void Camera::MouseMoved(MouseMovedEvent& event)
    {   
        float dX = event.GetX() - lastX;
        float dY = lastY - event.GetY();

        lastX = event.GetX();
        lastY = event.GetY();

        if (Application::GetWindow()->IsCursorEnabled()) return;

        const float sens = 0.1f;
        dX *= sens;
        dY *= sens;

        yaw += dX;

        while (yaw >= 360)
        {
            yaw -= 360;
        }
        while (yaw <= -360)
        {
            yaw += 360;
        }

        if (pitch + dY > 89.9f)
            pitch = 89.9f;
        else if (pitch + dY < -89.9f)
            pitch = -89.9f;
        else
			pitch += dY;
    }


}
