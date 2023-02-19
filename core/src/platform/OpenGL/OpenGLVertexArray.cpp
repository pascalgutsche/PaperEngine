#include "_Core.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace core
{
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


	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &vaoID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &vaoID);
	}

	void OpenGLVertexArray::ClearBuffers()
	{
		vertexBuffer->ClearBuffer();
		elementBuffer->ClearBuffer();
	}

	void OpenGLVertexArray::SetVertexBuffer(Shr<VertexBuffer>& vertexBuffer)
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

	void OpenGLVertexArray::SetElementBuffer(Shr<ElementBuffer>& elementBuffer)
	{
		Bind();
		elementBuffer->Bind();
		this->elementBuffer = elementBuffer;
	}

	void OpenGLVertexArray::Bind()
	{
		glBindVertexArray(vaoID);
	}

	void OpenGLVertexArray::Unbind()
	{
		glBindVertexArray(0);
	}

}
