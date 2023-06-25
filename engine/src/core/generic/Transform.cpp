#include "Engine.h"

#include "generic/Transform.h"

namespace ppr {

    Transform::Transform()
	    : position(glm::vec3(0.0f, 0.0f, 0.0f)), scale(glm::vec3(1.0f, 1.0f, 1.0f)), rotation(0.0f) { }

    Transform::Transform(glm::vec2 position, glm::vec2 scale, glm::vec3 rotation)
	    : position({ position, 0.0f }), scale({ scale, 1.0f }), rotation(rotation) { }

    Transform::Transform(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
	    : position(position), scale(scale), rotation(rotation) { }

    Transform::~Transform() { }

    Transform* Transform::Copy() const
    {
        return new Transform(glm::vec2(this->position), glm::vec2(this->scale));
    }

    void Transform::Copy(Transform& transform) const
    {
        transform.position = this->position;
        transform.scale = this->scale;
    }

    glm::mat4 Transform::GetMatrix() const
    {
        return glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f, 0.0f, 0.0f })
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f, 1.0f, 0.0f })
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), scale);
    }

    void Transform::Update()
    {
        rotation.x = (float)((int)(rotation.x * 10) % 3600) / 10.0f;
        rotation.y = (float)((int)(rotation.y * 10) % 3600) / 10.0f;
        rotation.z = (float)((int)(rotation.z * 10) % 3600) / 10.0f;
    }

    bool Transform::Equals(const Transform& transform) const {
        return this->position.x == transform.position.x && this->position.y == transform.position.y
            && this->scale.x == transform.scale.x && this->scale.y == transform.scale.y;
    }

}