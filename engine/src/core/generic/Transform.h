#pragma once
#include "Engine.h"
#include "utility.h"


namespace ppr {

    class Transform {
    public:
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;

        explicit Transform();
        explicit Transform(glm::vec2 position, glm::vec2 scale = { 1.0f, 1.0f }, glm::vec3 rotation = { 0.0f, 0.0f, 0.0f });
        explicit Transform(glm::vec3 position, glm::vec3 scale = { 1.0f, 1.0f, 1.0f }, glm::vec3 rotation = { 0.0f, 0.0f, 0.0f });
        ~Transform();

        glm::mat4 GetMatrix() const;

        Transform* Copy() const;
        // copy new values to transform
        void Copy(Transform& transform) const;
        bool Equals(const Transform& transform) const;

    private:
        friend class Entity;
        void Update();
    };

}