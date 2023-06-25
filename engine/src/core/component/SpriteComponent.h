#pragma once
#include "Engine.h"
#include "utility.h"

#include "renderer/Texture.h"

namespace engine {

	enum class Geometry
	{
		NONE,
		RECTANGLE,
		TRIANGLE
	};

	struct SpriteComponent {
		glm::vec4 color = glm::vec4(1.0f);
		Shr<Texture> texture = nullptr;
		float tiling_factor = 1.0f;
		std::array<glm::vec2, 4> tex_coords = { { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } } };
		Geometry geometry = Geometry::NONE;
		bool register_alpha_pixels_to_event = false;

		SpriteComponent(const glm::vec4 color, const Geometry geometry, const bool register_alpha_pixels_to_event = false)
			: color(color), geometry(geometry), register_alpha_pixels_to_event(register_alpha_pixels_to_event) { }

		SpriteComponent(const glm::vec4 color, Shr<Texture> texture, const Geometry geometry, const bool register_alpha_pixels_to_event = false)
			: color(color), texture(std::move(texture)), geometry(geometry), register_alpha_pixels_to_event(register_alpha_pixels_to_event) { }

		SpriteComponent(const glm::vec4 color, Shr<Texture> texture, const float tiling_factor, const Geometry geometry, const bool register_alpha_pixels_to_event = false)
			: color(color), texture(std::move(texture)), tiling_factor(tiling_factor), geometry(geometry), register_alpha_pixels_to_event(register_alpha_pixels_to_event) { }

	};

}