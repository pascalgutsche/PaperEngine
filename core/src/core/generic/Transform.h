#pragma once
#include "_Core.h"

namespace core {

    class Transform {
    public:
        glm::vec2 position;
        glm::vec2 scale;
        Transform();
        ~Transform();
        Transform(glm::vec2 position);
        Transform(glm::vec2 position, glm::vec2 scale);

        void init(glm::vec2 position, glm::vec2 scale);
        Transform* copy();
        // copy new values to transform
        void copy(Transform& transform);
        bool equals(Transform& transform);
    };

}