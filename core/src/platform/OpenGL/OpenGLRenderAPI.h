#pragma once
#include "_Core.h"
#include "utility.h"

#include "core/renderer/RenderAPI.h"

namespace core
{
	class OpenGLRenderAPI : public RenderAPI
	{
	public:
		void Init() override;
		void Clear(glm::vec4 color) override;
		void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		void DrawElements(Shr<VertexArray>& vertexArray) override;
	};
	
}

