#include "_Core.h"
#include "utility.h"
#include "LineRenderer.h"
#include "renderer/Renderer.h"

namespace core
{
	LineRenderer::LineRenderer(glm::vec2 positionA, glm::vec2 positionB, glm::vec4 color, float thickness)
	{
		Init(positionA, positionB, color, thickness);
	}

	void LineRenderer::Init(glm::vec2 positionA, glm::vec2 positionB, glm::vec4 color, float thickness)
	{
		this->positionA = positionA;
		this->positionB = positionB;
		this->color = color;
		this->thickness = thickness;
	}

	void LineRenderer::OnUpdate()
	{
		Renderer::DrawLine(positionA, positionB, gameObject->transform.rotation, color, thickness, gameObject->GetObjectID());
	}

	void LineRenderer::OnImgui(float dt)
	{

	}
};