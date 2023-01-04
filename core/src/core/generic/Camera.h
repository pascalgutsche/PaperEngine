#pragma once
#include "_Core.h"

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


    public:
        Camera();
        ~Camera();

        glm::vec3 target;
        glm::vec3 position;
        float fov;

        // update camera vectors
        void calcCameraVectors();

        glm::mat4 getViewMatrix();
        glm::mat4 getReverseViewMatrix();
        glm::mat4 getProjectionMatrix();
        glm::mat4 getOrthographicMatrix();
        glm::mat4 getReverseProjectionMatrix();
    };

}