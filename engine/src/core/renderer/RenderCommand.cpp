#include "Engine.h"
#include "RenderCommand.h"

#include "generic/Application.h"
#include "renderer/Renderer2D.h"
#include "renderer/Renderer3D.h"

namespace Paper
{
	Shr<RenderAPI> RenderCommand::rendererAPI = RenderAPI::CreateAPI();
	SharedRenderData RenderCommand::sharedData;

	void RenderCommand::Init()
	{
		rendererAPI->Init();
		

		sharedData.cameraUniformBuffer = UniformBuffer::CreateBuffer(0);

		Renderer2D::Init();
		Renderer3D::Init();
	}

	void RenderCommand::Shutdown()
	{
		Renderer2D::Shutdown();
		Renderer3D::Shutdown();
	}

	void RenderCommand::UploadCamera(const Shr<EditorCamera>& camera)
	{
		sharedData.cameraData.uOrthographic = camera->GetOrthographicMatrix();
		sharedData.cameraData.uPerspective = camera->GetProjectionMatrix();
		sharedData.cameraData.uView = camera->GetViewMatrix();
		sharedData.cameraUniformBuffer->SetData(&sharedData.cameraData, sizeof(SharedRenderData::CameraData));
	}

	SharedRenderData::Stats& RenderCommand::GetStats()
	{
		return sharedData.stats;
	}

	void RenderCommand::ClearStats()
	{
		memset(&sharedData.stats, 0, sizeof(SharedRenderData::Stats));
	}

	void RenderCommand::ClearColor(glm::vec4 color)
	{
		rendererAPI->SetClearColor(color);
	}

	void RenderCommand::Clear()
	{
		rendererAPI->Clear();
	}

	void RenderCommand::EnableDepthTesting(bool enable)
	{
		return rendererAPI->EnableDepthTesting(enable);
	}

	bool RenderCommand::IsDepthTestingEnabled()
	{
		return rendererAPI->IsDepthTestingEnabled();
	}

	void RenderCommand::DrawElements(Shr<VertexArray>& vertexArray, uint32_t elementCount)
	{
		rendererAPI->DrawElements(vertexArray, elementCount);
	}

	void RenderCommand::DrawLines(Shr<VertexArray>& vertexArray, uint32_t vertexCount, float thickness)
	{
		rendererAPI->DrawLines(vertexArray, vertexCount, thickness);
	}

	void RenderCommand::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		rendererAPI->SetViewPort(x, y, width, height);
	}

	void RenderCommand::SetLineThickness(float width)
	{
		rendererAPI->SetLineWidth(width);
	}

	void RenderCommand::SetPolygonModel(Polygon pol)
	{
		rendererAPI->SetPolygonModel(pol);
	}
}
