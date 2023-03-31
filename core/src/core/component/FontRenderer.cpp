#include "_Core.h"
#include "FontRenderer.h"

#include "renderer/Renderer.h"

namespace core
{

	FontRenderer::FontRenderer(glm::vec2 position, glm::vec2 size, glm::vec4 color, std::string fontPath, std::string text, ProjectionMode mode)
	{
		Init(position, size, color, fontPath, text, mode);
	}

	FontRenderer::FontRenderer(glm::vec2 position, glm::vec2 size, glm::vec4 color, std::string text, ProjectionMode mode)
	{
		Init(position, size, color, "assets/fonts/mononoki.ttf", text, mode);
	}

	void FontRenderer::Init(glm::vec2 position, glm::vec2 size, glm::vec4 color, std::string fontPath, std::string text, ProjectionMode mode)
	{
		// calculate shit
		this->position = position;
		this->size = size;
		this->color = color;
		this->fontPath = fontPath;
		this->text = text;
		this->mode = mode;
	}

	void FontRenderer::ChangeText(std::string text)
	{
		this->text = text;
	}

	void FontRenderer::OnUpdate()
	{
		Renderer::DrawString(position, size, color, text, Font::GetFont(fontPath), mode, gameObject->GetObjectID());
	}

	void FontRenderer::OnImgui(float dt)
	{

	}
};