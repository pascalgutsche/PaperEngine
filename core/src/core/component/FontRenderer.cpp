#include "_Core.h"
#include "FontRenderer.h"

#include "renderer/Renderer.h"

namespace core
{

	FontRenderer::FontRenderer(glm::vec4 color, std::string fontPath, std::string text, ProjectionMode mode)
	{
		Init(color, fontPath, text, mode);
	}

	FontRenderer::FontRenderer(glm::vec4 color, std::string text, ProjectionMode mode)
	{
		Init(color, "assets/fonts/mononoki.ttf", text, mode);
	}

	void FontRenderer::Init(glm::vec4 color, std::string fontPath, std::string text, ProjectionMode mode)
	{
		// calculate shit
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
		Renderer::DrawString(gameObject->transform.position, gameObject->transform.scale, gameObject->transform.rotation, color, text, Font::GetFont(fontPath), mode, gameObject->GetObjectID());
	}

	void FontRenderer::OnImgui(float dt)
	{

	}
};