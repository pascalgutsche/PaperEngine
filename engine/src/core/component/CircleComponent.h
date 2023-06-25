#pragma once
#include "Engine.h"

#include "renderer/Texture.h"

namespace ppr
{
    struct CircleComponent {
        glm::vec4 color = glm::vec4(1.0f);
        Shr<Texture> texture = nullptr;
        float tiling_factor = 1.0f;
        float thickness = 1.0f;
        float fade = 0.005f;
        bool register_alpha_pixels_to_event = false;

        CircleComponent(glm::vec4 color, float thickness, float fade, bool register_alpha_pixels_to_event = false)
	        : color(color), thickness(thickness), fade(fade), register_alpha_pixels_to_event(register_alpha_pixels_to_event) { }
        CircleComponent(glm::vec4 color, const Shr<Texture>& texture, float thickness, float fade, bool register_alpha_pixels_to_event = false)
	        : color(color), texture(texture), thickness(thickness), fade(fade), register_alpha_pixels_to_event(register_alpha_pixels_to_event) { }
        CircleComponent(glm::vec4 color, const Shr<Texture>& texture, float tilingFactor, float thickness, float fade, bool register_alpha_pixels_to_event = false)
            : color(color), texture(texture), tiling_factor(tilingFactor), thickness(thickness), fade(fade), register_alpha_pixels_to_event(register_alpha_pixels_to_event) { }

    };
}


