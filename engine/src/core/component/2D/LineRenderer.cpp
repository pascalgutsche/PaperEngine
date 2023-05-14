#include "Engine.h"
#include "utility.h"
#include "component/2D/LineRenderer.h"
#include "renderer/Renderer2D.h"
#include "layer/Layer.h"

namespace engine
{
	LineRenderer::LineRenderer(glm::vec3 positionA, glm::vec3 positionB, glm::vec4 color, float thickness, bool registerAlphaPixelsToEvent)
		:positionA(positionA), positionB(positionB), color(color), thickness(thickness), registerAlphaPixelsToEvent(registerAlphaPixelsToEvent) { }

	void LineRenderer::OnRender()
	{
		LineRenderData data;
		data.point0 = positionA;
		data.point1 = positionB;
		data.color = color;
		data.thickness = thickness;
		data.mode = gameObject->GetProjectionMode();
		data.coreID = gameObject->GetCoreID();
		Renderer2D::DrawLine(data);
	}
};