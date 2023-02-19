#pragma once
#include "_Core.h"
#include "utility.h"

#include "core/renderer/Buffer.h"

namespace core
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		void Bind() override;
		void Unbind() override;

		void Add(std::vector<float> data, uint32_t id) override;
		void Update(std::vector<float> data, uint32_t id) override;
		void Remove(uint32_t id) override;

		void BufferSubData() override;

		const BufferLayout& GetLayout() override;

		OpenGLVertexBuffer(BufferLayout& layout, unsigned int size);
		~OpenGLVertexBuffer() override;
	private:
		size_t size;
		BufferLayout layout;
		std::vector<Vertex*> vertices;
		std::vector<float> rawVertices;
		bool reloadData = false;
		uint32_t vboID;
		void ConvertVerticesToRawData();
	};

	class OpenGLElementBuffer : public ElementBuffer
    {
    public:
		void Bind() override;
		void Unbind() override;

		void Add(std::vector<unsigned int> data, uint32_t id) override;
	    void Update(std::vector<unsigned int> data, uint32_t id) override;
	    void Remove(uint32_t id) override;

	    void BufferSubData() override;
		
	    unsigned int GetElementCount() override;
		 
		OpenGLElementBuffer(size_t size);
	    ~OpenGLElementBuffer() override;
    private:
    	size_t size;

		std::vector<Element*> elements;
		std::vector<unsigned int> rawElements;
		bool reloadData = false;

		uint32_t eboID;

		void ConvertElementsToRawData();
    };
}