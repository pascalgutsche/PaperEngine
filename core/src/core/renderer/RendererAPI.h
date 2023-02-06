#pragma once
#include "_Core.h"
#include "utility.h"

#include "renderer/VertexArray.h"

namespace core {

	class RendererAPI
	{
	public:
		void Init();
		void Clear(glm::vec4 color);

		void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		void DrawElements(Shr<VertexArray>& vertexArray);

		static Shr<RendererAPI> CreateAPI();
	};

}

