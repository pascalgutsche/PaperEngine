#pragma once
#include "Engine.h"
#include "generic/Component.h"

namespace engine
{
    class CircleRenderer : public RenderComponent {
    public:
        CircleRenderer(glm::vec4 color, float thickness, float fade, bool registerAlphaPixelsToEvent = false);
        CircleRenderer(glm::vec4 color, Shr<Texture> texture, float thickness, float fade, bool registerAlphaPixelsToEvent = false);

        ~CircleRenderer() override { }

        void OnStart() override { }
        void OnStop() override { }
        void OnUpdate() override { };
        void OnEvent(Event& event) override { }
        void OnRender() override;

        void SetColor(glm::vec4 color) { this->color = color; }

    private:
        glm::vec4 color;
        Shr<Texture> texture;
        float thickness;
        float fade;
        bool registerAlphaPixelsToEvent;
    };
}


