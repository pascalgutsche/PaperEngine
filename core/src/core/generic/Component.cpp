#include "_Core.h"

#include "generic/GameObject_Component.h"

namespace core {

    //COMPONENT
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