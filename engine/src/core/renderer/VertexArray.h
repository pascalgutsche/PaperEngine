#pragma once
#include "Engine.h"
#include "utility.h"

#include "renderer/RenderBuffer.h"


namespace Paper
{
	class VertexArray
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetVertexBuffer(Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetElementBuffer(Ref<ElementBuffer>& elementBuffer) = 0;

		virtual Ref<VertexBuffer>& GetVertexBuffer() = 0;
		virtual Ref<ElementBuffer>& GetElementBuffer() = 0;

		virtual ~VertexArray() = default;

		static Ref<VertexArray> CreateArray();
	};
}

