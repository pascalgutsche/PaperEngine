#include "_Core.h"
#include "FontRenderer.h"

#include "renderer/Renderer.h"

namespace core
{

	FontRenderer::FontRenderer(glm::vec4 color, std::string fontPath, std::string text)
	{
		Init(color, fontPath, text);
	}

	FontRenderer::FontRenderer(glm::vec4 color, std::string text)
	{
		Init(color, "assets/fonts/mononoki.ttf", text);
	}

	void FontRenderer::Init(glm::vec4 color, std::string fontPath, std::string text)
	{
		// calculate shit
		this->color = color;
		this->fontPath = fontPath;
		this->text = text;
	}

	void FontRenderer::ChangeText(std::string text)
	{
		this->text = text;
	}

	void FontRenderer::OnUpdate()
	{
		Renderer::DrawString(gameObject->transform.position, gameObject->transform.scale, gameObject->transform.rotation, color, text, Font::GetFont(fontPath), gameObject->GetProjectionMode(), gameObject->GetObjectID());
	}

	void FontRenderer::OnImgui(float dt)
	{

	}
};