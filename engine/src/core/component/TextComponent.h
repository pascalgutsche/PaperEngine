#pragma once
#include "generic/Component.h"
#include "utils/DataPool.h"

namespace engine
{
	struct TextComponent
	{
		glm::vec4 color = glm::vec4(1.0f);
		std::string text = "";
		Shr<Font> font = DataPool::GetFont("mononoki.ttf");
		bool register_alpha_pixels_to_event = false;

		TextComponent(const glm::vec4 color, const std::string& text, const Shr<Font>& font = DataPool::GetFont("mononoki.ttf"), const bool registerAlphaPixelsToEvent = false)
			: color(color), text(text), font(font), register_alpha_pixels_to_event(registerAlphaPixelsToEvent) {}
	};
};


