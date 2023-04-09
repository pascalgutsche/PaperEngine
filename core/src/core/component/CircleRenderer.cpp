#include "_Core.h"

#include "component/CircleRenderer.h"
#include "generic/GameObject.h"
#include "renderer/Renderer.h"


namespace core {

    CircleRenderer::CircleRenderer(glm::vec4 color, float thickness, float fade)
    {
        this->color = color;
        this->thickness = thickness;
        this->fade = fade;
    }

    void CircleRenderer::OnUpdate() 
    {
        Renderer::DrawCircle(gameObject->transform.position, gameObject->transform.scale, gameObject->transform.rotation, color, thickness, fade, gameObject->GetProjectionMode());
    }
}