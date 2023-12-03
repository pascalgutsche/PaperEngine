#pragma once
#include "Engine.h"
#include "utility.h"

#include "renderer/VertexArray.h"
#include "renderer/RenderAPI.h"
#include "camera/EditorCamera.h"
#include "camera/EntityCamera.h"

namespace Paper
{
	struct SharedRenderData
	{

		struct CameraData
		{
			glm::mat4 uProjection;
			glm::mat4 uView;
		};
		CameraData cameraData;
		Ref<UniformBuffer> cameraUniformBuffer;

		struct Stats
		{
			uint32_t drawCalls = 0;
			uint32_t dataSize = 0;
			uint32_t objectCount = 0;
			uint32_t vertexCount = 0;
			uint32_t elementCount = 0;
		};
		Stats stats;
	};

	class RenderCommand
	{
	private:
		static Ref<RenderAPI> rendererAPI;

	public:
		static SharedRenderData sharedData;

		static void Init();
		static void Shutdown();

		static void UploadCamera(const Ref<EditorCamera>& camera);
		static void UploadCamera(const EntityCamera& entityCamera, const glm::mat4& viewMatrix);
		static SharedRenderData::Stats& GetStats();
		static void ClearStats();

		static void ClearColor(glm::vec4 color);
		static void Clear();
		static void EnableDepthTesting(bool enable);
		static bool IsDepthTestingEnabled();
		static void DrawElements(Ref<VertexArray>& vertexArray, uint32_t elementCount);
		static void DrawLines(Ref<VertexArray>& vertexArray, uint32_t vertexCount, float thickness);
		static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetLineThickness(float width);
		static void SetPolygonModel(Polygon pol);
	};

}

