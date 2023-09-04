#include "Engine.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Paper
{
	//
	// VERTEX BUFFER
	//

	OpenGLVertexBuffer::OpenGLVertexBuffer(BufferLayout& layout, uint32_t size)
		: layout(layout)
	{
		glCreateBuffers(1, &vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &vboID);
	}

	void OpenGLVertexBuffer::AddData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	const BufferLayout& OpenGLVertexBuffer::GetLayout()
	{
		return this->layout;
	}

	void OpenGLVertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
	}

	void OpenGLVertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//
	// ELEMENT BUFFER
	//

	OpenGLElementBuffer::OpenGLElementBuffer(uint32_t* data, uint32_t count)
		:count(count)
	{
		glCreateBuffers(1, &eboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_DYNAMIC_DRAW);
	}

	OpenGLElementBuffer::~OpenGLElementBuffer()
	{
		glDeleteBuffers(1, &eboID);
	}

	unsigned OpenGLElementBuffer::GetElementCount()
	{
		return count;
	}

	void OpenGLElementBuffer::Bind() 
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	}

	void OpenGLElementBuffer::Unbind() 
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


	//
	// UNIFORM BUFFER
	//

	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t binding)
		: uboID(0), size(0), binding(binding)
	{
		Invalidate(size);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		if (uboID)
			glDeleteBuffers(1, &uboID);
	}

	void OpenGLUniformBuffer::Invalidate(uint32_t size)
	{
		if (uboID)
			glDeleteBuffers(1, &uboID);

		glCreateBuffers(1, &uboID);
		glBindBuffer(GL_UNIFORM_BUFFER, uboID);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, uboID);

		this->size = size;
	}

	void OpenGLUniformBuffer::SetData(void* data, uint32_t size)
	{
		if (this->size < size)
			Invalidate(size);

		Bind();
		glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
	}

	void OpenGLUniformBuffer::Bind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, uboID);
	}

	void OpenGLUniformBuffer::Unbind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}


	//
	// STORAGE BUFFER
	//

	OpenGLStorageBuffer::OpenGLStorageBuffer(uint32_t binding)
		: ssboID(0), size(0), binding(binding)
	{
		Invalidate(size);
	}

	OpenGLStorageBuffer::~OpenGLStorageBuffer()
	{
		if (ssboID)
			glDeleteBuffers(1, &ssboID);
	}

	void OpenGLStorageBuffer::Invalidate(uint32_t size)
	{
		if (ssboID)
			glDeleteBuffers(1, &ssboID);

		glCreateBuffers(1, &ssboID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, ssboID);

		this->size = size;
	}

	void OpenGLStorageBuffer::SetData(void* data, uint32_t objectCount, uint32_t objectSize)
	{
		if (this->size < objectSize * objectCount + sizeof(uint32_t))
			Invalidate(objectSize * objectCount + sizeof(uint32_t));

		Bind();
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(uint32_t), &objectCount);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(uint32_t), objectSize * objectCount, data);
	}

	void OpenGLStorageBuffer::Bind()
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboID);
	}

	void OpenGLStorageBuffer::Unbind()
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}
