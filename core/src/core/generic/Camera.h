#pragma once
#include "_Core.h"
#include "utility.h"

#include "event/MouseEvent.h"

namespace core {

    class Camera {
    private:
        // Z
        glm::vec3 cameraDirection;
        // X
        glm::vec3 cameraRight;
        // Y
        glm::vec3 cameraUp;

        glm::mat4 cameraViewMatrix;
        glm::mat4 cameraProjectionMatrix;
        glm::mat4 model;

        glm::vec3 worldUp;
        glm::vec3 front;

        glm::vec3 direction;

        float lastX, lastY;

    public:
        Camera();
        ~Camera();

        glm::vec3 target;
        glm::vec3 position;
        float fov;

        float pitch;
        float yaw;
        float roll;

        // update camera vectors
        void CalcCameraVectors();

        glm::mat4 GetViewMatrix();
        glm::mat4 GetReverseViewMatrix();
        glm::mat4 GetProjectionMatrix();
        glm::mat4 GetOrthographicMatrix();
        glm::mat4 GetReverseProjectionMatrix();

        glm::vec3 GetFront() const;

        void MouseMoved(MouseMovedEvent& event);
    };

}