#pragma once
#include "_Core.h"

#include "utility.h"

namespace core {

    class CORE_API Transform {
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