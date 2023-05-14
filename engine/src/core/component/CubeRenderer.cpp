#include "Engine.h"
#include "CubeRenderer.h"

#include "renderer/Renderer3D.h"
#include "component/LightComponent.h"

namespace engine
{
	CubeRenderer::CubeRenderer(glm::vec4 color)
		: color(color), texture(nullptr), tilingFactor(1.0f) { }

	CubeRenderer::CubeRenderer(glm::vec4 color, Shr<Texture> texture)
		: color(color), texture(texture), tilingFactor(1.0f) { }

	CubeRenderer::CubeRenderer(glm::vec4 color, Shr<Texture> texture, float tilingFactor)
		: color(color), texture(texture), tilingFactor(tilingFactor) { }

	void CubeRenderer::OnRender()
	{
		EdgeRenderData3D data;
		data.transform = gameObject->transform;
		data.color = color;
		if (gameObject->GetComponent<LightComponent>())
			data.isLightSource = true;
		data.texture = texture;
		data.tilingFactor = tilingFactor;
		Renderer3D::DrawCube(data);
	}
}
