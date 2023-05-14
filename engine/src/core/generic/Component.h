#pragma once
#include "Engine.h"
#include "utility.h"

#include "event/Event.h"

#include "generic/Entity.h"

namespace engine {

    class Component {
    protected:
        Entity* gameObject = nullptr;

    public:
        Component() = default;
    	virtual ~Component() = default;

        virtual void OnStart() = 0;
        virtual void OnStop() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnEvent(Event& event) = 0;
        virtual void OnImgui(float dt) { }

        Entity* GetEntity() const { return gameObject; }

        friend class Entity;
    };

    class RenderComponent : public Component {
    public:
        RenderComponent() = default;
        virtual ~RenderComponent() override = default;

        virtual void OnStart() override = 0;
        virtual void OnStop() override = 0;
        virtual void OnUpdate() override = 0;
        virtual void OnRender() = 0;
        virtual void OnEvent(Event& event) override = 0;
        virtual void OnImgui(float dt) override { }


        friend class Entity;
    };

}