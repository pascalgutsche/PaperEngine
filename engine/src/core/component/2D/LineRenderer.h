#pragma once
#include "Engine.h"
#include "generic/Component.h"

namespace engine
{
	class LineRenderer : public RenderComponent
	{
    public:
        LineRenderer(glm::vec3 pointA, glm::vec3 pointB, glm::vec4 color, float thickness, bool registerAlphaPixelsToEvent = false);

        void OnStart() override { }
        void OnStop() override { }
        void OnUpdate() override { };
        void OnEvent(Event& event) override { }
        void OnRender() override;

    private:
        glm::vec3 positionA;
        glm::vec3 positionB;
        glm::vec4 color;

        float thickness;

        bool registerAlphaPixelsToEvent;
	};
};