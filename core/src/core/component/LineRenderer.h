#pragma once
#include "_Core.h"
#include "generic/Component.h"

namespace core
{
	class LineRenderer : public RenderComponent
	{
    public:
        LineRenderer(glm::vec2 pointA, glm::vec2 pointB, glm::vec4 color, float thickness, bool registerAlphaPixelsToEvent = false);

        void OnStart() override { }
        void OnStop() override { }
        void OnUpdate() override;
        void OnEvent(Event& event) override { }

    private:
        glm::vec2 positionA;
        glm::vec2 positionB;
        glm::vec4 color;

        float thickness;

        bool registerAlphaPixelsToEvent;
	};
};