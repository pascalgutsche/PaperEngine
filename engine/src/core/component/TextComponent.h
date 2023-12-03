#pragma once
#include "utils/DataPool.h"

#include "Serializable.h"

namespace Paper
{
	struct TextComponent : Serializable
	{
		glm::vec4 color = glm::vec4(1.0f);
		std::string text = "][DEFAULT-TEXT][";
		Ref<Font> font = DataPool::GetDefaultFont();
		bool register_alpha_pixels_to_event = false;

		TextComponent() = default;
		~TextComponent() override = default;

		TextComponent(const glm::vec4 color, const std::string& text, const Ref<Font>& font = DataPool::GetFont("mononoki.ttf"), const bool registerAlphaPixelsToEvent = false)
			: color(color), text(text), font(font), register_alpha_pixels_to_event(registerAlphaPixelsToEvent) {}

		bool Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node& data) override;
	};
};


