#pragma once
#include "Engine.h"
#include "utility.h"

#include "core/renderer/Buffer.h"

namespace ppr
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		void Bind() override;
		void Unbind() override;

		void AddData(const void* data, uint32_t size) override;

		const BufferLayout& GetLayout() override;

		OpenGLVertexBuffer(BufferLayout& layout, uint32_t size);
		~OpenGLVertexBuffer() override;
	private:
		uint32_t vboID;
		BufferLayout layout;
	};

	class OpenGLElementBuffer : public ElementBuffer
    {
    public:
		void Bind() override;
		void Unbind() override;
		
	    unsigned int GetElementCount() override;
		 
		OpenGLElementBuffer(uint32_t* data, uint32_t count);
	    ~OpenGLElementBuffer() override;
    private:
		uint32_t eboID;
    	uint32_t count;
    };

	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		void Bind() override;
		void Unbind() override;

		void SetData(void* data, uint32_t size) override;

		OpenGLUniformBuffer(uint32_t binding);
		~OpenGLUniformBuffer() override;
	private:
		void Invalidate(uint32_t size);

		uint32_t uboID;
		uint32_t size;
		uint32_t binding;
	};

	class OpenGLStorageBuffer : public StorageBuffer
	{
	public:
		void Bind() override;
		void Unbind() override;

		void SetData(void* data, uint32_t objectCount, uint32_t objectSize) override;

		OpenGLStorageBuffer(uint32_t binding);
		~OpenGLStorageBuffer() override;
	private:
		void Invalidate(uint32_t size);

		uint32_t ssboID;
		uint32_t size;
		uint32_t binding;
	};
}