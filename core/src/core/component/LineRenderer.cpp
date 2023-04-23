#include "_Core.h"
#include "utility.h"
#include "LineRenderer.h"
#include "renderer/Renderer.h"
#include "layer/Layer.h"

namespace core
{
	LineRenderer::LineRenderer(glm::vec2 positionA, glm::vec2 positionB, glm::vec4 color, float thickness)
		:positionA(positionA), positionB(positionB), color(color), thickness(thickness) { }

	void LineRenderer::OnUpdate()
	{
		LineRenderData data;
		data.point0 = positionA;
		data.point1 = positionB;
		data.color = color;
		data.thickness = thickness;
		data.mode = gameObject->GetProjectionMode();
		data.coreID = gameObject->GetCoreID();
		Renderer::DrawLine(data);
	}
};