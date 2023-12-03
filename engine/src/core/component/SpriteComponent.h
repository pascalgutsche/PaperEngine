#pragma once
#include "Engine.h"
#include "utility.h"

#include "renderer/Texture.h"

#include "Serializable.h"
#include "utils/DataPool.h"

namespace Paper {

	enum class Geometry
	{
		NONE,
		TRIANGLE,
		RECTANGLE,
		CIRCLE,

		_LAST = CIRCLE
	};

	inline std::string GeometryToString(const Geometry geometry)
	{
		switch (geometry) {
			case Geometry::NONE: return "None";
			case Geometry::TRIANGLE: return "Triangle";
			case Geometry::RECTANGLE: return "Rectangle";
			case Geometry::CIRCLE: return "Circle";
		}
		CORE_ASSERT(false, "")
		return "";
	}

	inline Geometry StringToGeometry(std::string geometry)
	{
		if (geometry == "None") return Geometry::NONE;
		if (geometry == "Triangle") return Geometry::TRIANGLE;
		if (geometry == "Rectangle") return Geometry::RECTANGLE;
		if (geometry == "Circle") return Geometry::CIRCLE;
		CORE_ASSERT(false, "");
		return Geometry::NONE;
	}

	struct SpriteComponent : Serializable {
		glm::vec4 color = glm::vec4(1.0f);
		Ref<Texture> texture = nullptr;
		float tiling_factor = 1.0f;
		std::array<glm::vec2, 4> tex_coords = { { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } } };
		Geometry geometry = Geometry::RECTANGLE;
		bool register_alpha_pixels_to_event = false;

		//only for circles
		float thickness = 1.0f;
		float fade = 0.005f;

		SpriteComponent() = default;
		~SpriteComponent() override = default;

		SpriteComponent(const glm::vec4 color, const Geometry geometry, const bool register_alpha_pixels_to_event = false)
			: color(color), geometry(geometry), register_alpha_pixels_to_event(register_alpha_pixels_to_event) { }

		SpriteComponent(const glm::vec4 color, const Geometry geometry, float thickness, float fade = 0.005f, const bool register_alpha_pixels_to_event = false)
			: color(color), geometry(geometry), thickness(thickness), fade(fade), register_alpha_pixels_to_event(register_alpha_pixels_to_event) { }

		SpriteComponent(const glm::vec4 color, Ref<Texture> texture, const Geometry geometry, const bool register_alpha_pixels_to_event = false)
			: color(color), texture(std::move(texture)), geometry(geometry), register_alpha_pixels_to_event(register_alpha_pixels_to_event) { }

		SpriteComponent(const glm::vec4 color, Ref<Texture> texture, const Geometry geometry, float thickness, float fade = 0.005f, const bool register_alpha_pixels_to_event = false)
			: color(color), texture(std::move(texture)), geometry(geometry), thickness(thickness), fade(fade), register_alpha_pixels_to_event(register_alpha_pixels_to_event) { }

		SpriteComponent(const glm::vec4 color, Ref<Texture> texture, const float tiling_factor, const Geometry geometry, const bool register_alpha_pixels_to_event = false)
			: color(color), texture(std::move(texture)), tiling_factor(tiling_factor), geometry(geometry), register_alpha_pixels_to_event(register_alpha_pixels_to_event) { }

		SpriteComponent(const glm::vec4 color, Ref<Texture> texture, const float tiling_factor, const Geometry geometry, float thickness, float fade = 0.005f, const bool register_alpha_pixels_to_event = false)
			: color(color), texture(std::move(texture)), tiling_factor(tiling_factor), geometry(geometry), thickness(thickness), fade(fade), register_alpha_pixels_to_event(register_alpha_pixels_to_event) { }

		bool Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node& data) override;
	};

}
