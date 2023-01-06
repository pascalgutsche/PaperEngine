#include "_Core.h"

#include "generic/Transform.h"

namespace core {

    // transform is basically adjusting the size and position variables of the given transform object
    Transform::Transform()
    {
        // default transform constructor sets empty values
        init(glm::vec2(), glm::vec2(1.0f, 1.0f));
    }

    Transform::~Transform()
    {

    }

    Transform* Transform::copy()
    {
        return new Transform(glm::vec2(this->position), glm::vec2(this->scale));
    }

    void Transform::copy(Transform& transform)
    {
        transform.position = this->position;
        transform.scale = this->scale;
    }

    Transform::Transform(glm::vec2 position)
    {
        init(position, glm::vec2(1.0f, 1.0f));
    }

    Transform::Transform(glm::vec2 position, glm::vec2 scale)
    {
        init(position, scale);
    }

    void Transform::init(glm::vec2 position, glm::vec2 scale)
    {
        Transform::position = position;
        Transform::scale = scale;
    }

    bool Transform::equals(Transform& transform) {
        return this->position.x == transform.position.x && this->position.y == transform.position.y
            && this->scale.x == transform.scale.x && this->scale.y == transform.scale.y;
    }

}