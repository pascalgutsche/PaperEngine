#pragma once
#include "Engine.h"
#include "generic/Component.h"

#include "renderer/Texture.h"

namespace engine
{
    class CircleRenderer : public RenderComponent {
    public:
        glm::vec4 color = glm::vec(1.0f);
        Shr<Texture> texture = nullptr;
        float thickness = 1.0f;
        float fade = 0.005f;
        bool register_alpha_pixels_to_event = false;

        CircleRenderer(glm::vec4 color, float thickness, float fade, bool register_alpha_pixels_to_event = false);
        CircleRenderer(glm::vec4 color, Shr<Texture> texture, float thickness, float fade, bool register_alpha_pixels_to_event = false);

        ~CircleRenderer() override { }

        void OnStart() override { }
        void OnStop() override { }
        void OnUpdate() override { };
        void OnEvent(Event& event) override { }
        void OnRender() override;

        void SetColor(glm::vec4 color) { this->color = color; }

    private:

    };
}


