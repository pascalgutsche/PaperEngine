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

    void Component::update(float dt) {

    }

    void Component::imgui(float dt) {

    }

    std::string Component::getTypeID() {
        return this->typeID;
    }

}