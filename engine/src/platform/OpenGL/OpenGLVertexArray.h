#pragma once
#include "Engine.h"
#include "utility.h"

#include "renderer/VertexArray.h"

namespace Paper
{
    class OpenGLVertexArray : public VertexArray
	{
	public:
		void Bind() override;
		void Unbind() override;

		void SetVertexBuffer(Ref<VertexBuffer>& vertexBuffer) override;
		void SetElementBuffer(Ref<ElementBuffer>& elementBuffer) override;

		Ref<VertexBuffer>& GetVertexBuffer()override { return vertexBuffer; }
		Ref<ElementBuffer>& GetElementBuffer() override { return elementBuffer; }

        OpenGLVertexArray();
		~OpenGLVertexArray() override;
	private:
		uint32_t vaoID;
		uint32_t vboIndex = 0;

		Ref<VertexBuffer> vertexBuffer;
		Ref<ElementBuffer> elementBuffer;
	};
}