#include "_Core.h"

#include "generic/Component.h"

namespace core {

    Component::Component() {
        typeID = std::string("unknown");
    }

    Component::~Component() {

    }

    void Component::start() {

    }

    void Component::update(float deltaTime) {

    }

    void Component::imgui(float deltaTime) {

    }

    std::string Component::getTypeID() {
        return this->typeID;
    }

}