#include "_Core.h"
#include "VertexArray.h"

#include "glad/glad.h"

namespace core {

	static GLenum GLSLDataTypeToOpenGlBaseType(GLSLDataType type)
	{
		switch (type) {
			case GLSLDataType::FLOAT2:  return GL_FLOAT;
			case GLSLDataType::FLOAT:	return GL_FLOAT;
			case GLSLDataType::FLOAT3:  return GL_FLOAT;
			case GLSLDataType::FLOAT4:  return GL_FLOAT;
			case GLSLDataType::INT:		return GL_FLOAT;
			case GLSLDataType::INT2:	return GL_FLOAT;
			case GLSLDataType::INT3:	return GL_INT;
			case GLSLDataType::INT4:	return GL_INT;
			case GLSLDataType::MAT3:	return GL_INT;
			case GLSLDataType::MAT4:	return GL_INT;
			case GLSLDataType::BOOL:	return GL_BOOL;
			default:;
		}

		CORE_ASSERT(false, "unknown GLSLDataType");
		return 0;
	}

	Shr<VertexArray> VertexArray::CreateArray()
	{
		return MakeShr<VertexArray>();
	}

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &vaoID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &vaoID);
	}

	void VertexArray::SetVertexBuffer(const Shr<VertexBuffer>& vertexBuffer)
	{
		Bind();
		vertexBuffer->Bind();

		CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "vertex buffer has no layout");

		uint32_t index = 0;
		const BufferLayout& layout = vertexBuffer->GetLayout();
		for (const BufferElement& element : layout)
		{
			glVertexAttribPointer(index,
				element.count,
				GLSLDataTypeToOpenGlBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.offset);
			glEnableVertexAttribArray(index);
			index++;
		}

		this->vertexBuffer = vertexBuffer;
	}

	void VertexArray::SetElementBuffer(const Shr<ElementBuffer>& elementBuffer)
	{

		Bind();
		elementBuffer->Bind();
		this->elementBuffer = elementBuffer;
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(vaoID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

}


