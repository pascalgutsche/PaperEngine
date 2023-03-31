#pragma once
#include "generic/Component.h"

namespace core
{
	class FontRenderer : public Component
	{
	public:
		FontRenderer(glm::vec2 position, glm::vec2 size, glm::vec4 color, std::string fontPath, std::string text, ProjectionMode mode);
		FontRenderer(glm::vec2 position, glm::vec2 size, glm::vec4 color, std::string text, ProjectionMode mode);

		void Init(glm::vec2 position, glm::vec2 size, glm::vec4 color, std::string fontPath, std::string text, ProjectionMode mode);

		void ChangeText(std::string text);

		void OnStart() override { }
		void OnStop() override { }
		void OnUpdate() override;
		void OnImgui(float dt) override;
		void OnEvent(Event& event) override { }

	private:
		glm::vec2 position;
		glm::vec2 size;
		glm::vec4 color;
		std::string fontPath;
		std::string text;

		ProjectionMode mode;
	};
};


