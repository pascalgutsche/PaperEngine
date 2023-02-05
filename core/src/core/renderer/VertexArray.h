#pragma once
#include "_Core.h"
#include "utility.h"

#include "renderer/Buffer.h"


namespace core
{
	class VertexArray
	{
	public:
		void Bind() const;
		void Unbind() const;

		void SetVertexBuffer(const Shr<VertexBuffer>& vertexBuffer);
		void SetElementBuffer(const Shr<ElementBuffer>& elementBuffer);

		const Shr<VertexBuffer>& GetVertexBuffer() const { return vertexBuffer; }
		const Shr<ElementBuffer>& GetElementBuffer() const { return elementBuffer; }

		static Shr<VertexArray> CreateArray();

		VertexArray();
		~VertexArray();
	private:
		uint32_t vaoID;
		uint32_t vboIndex = 0;

		Shr<VertexBuffer> vertexBuffer;
		Shr<ElementBuffer> elementBuffer;
	};
}

