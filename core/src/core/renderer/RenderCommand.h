#pragma once
#include "_Core.h"
#include "utility.h"

#include "renderer/VertexArray.h"
#include "renderer/RenderAPI.h"
#include "renderer/FrameBuffer.h"

namespace core {

	class Camera;

	struct SharedRenderData
	{

		struct CameraData
		{
			glm::mat4 uPerspective;
			glm::mat4 uOrthographic;
			glm::mat4 uView;
		};
		CameraData cameraData;
		Shr<UniformBuffer> cameraUniformBuffer;
	};

	class RenderCommand
	{
	private:
		static Shr<RenderAPI> rendererAPI;
		static Shr<Framebuffer> framebuffer;

	public:
		static SharedRenderData sharedData;

		static void Init();

		static void UploadCamera(const Shr<Camera>& camera);

		static void ClearColor(glm::vec4& color);
		static void Clear();
		static void PrepareFramebuffer();
		static void EnableDepthTesting(bool enable);
		static bool IsDepthTestingEnabled();
		static void DrawElements(Shr<VertexArray>& vertexArray, uint32_t elementCount);
		static void DrawLines(Shr<VertexArray>& vertexArray, uint32_t vertexCount, float thickness);
		static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetLineThickness(float width);
		static void SetPolygonModel(Polygon pol);
		static Shr<Framebuffer> GetFramebuffer();
	};

}

