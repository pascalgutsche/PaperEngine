#include "_Core.h"
#include "Buffer.h"

#include <glad/glad.h>

namespace core
{
	//
	// VERTEX BUFFER
	//

	Shr<VertexBuffer> VertexBuffer::CreateBuffer(BufferLayout layout, unsigned size)
	{
		return MakeShr<VertexBuffer>(layout, size);
	}

	VertexBuffer::VertexBuffer(BufferLayout layout, unsigned size)
		: size(size), layout(layout)
	{
		glCreateBuffers(1, &vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

	}

	VertexBuffer::~VertexBuffer()
	{
		for (Vertex* vertex : vertices)
		{
			delete vertex;
		}
		glDeleteBuffers(1, &vboID);
	}

	

	uint32_t VertexBuffer::Add(std::vector<float> data, uint32_t id)
	{
		Vertex* vertex = new Vertex;
		vertex->data = data;
		vertex->numberOfVertices = data.size() / layout.GetSize();
		vertex->id = id;
		vertex->vertexOffset = CalculateOffset(vertex);
		this->vertices.emplace_back(vertex);
		ConvertVerticesToRawData();
		return vertex->vertexOffset;
	}

	void VertexBuffer::Update(std::vector<float> data, uint32_t id)
	{
		for (Vertex* vertex : vertices)
		{
			if (vertex->id == id)
			{
				vertex->data = data;
			}
		}
		ConvertVerticesToRawData();
	}

	void VertexBuffer::Remove(uint32_t id)
	{
		Vertex* temp;
		for (Vertex* vertex : vertices)
		{
			if (vertex->id == id)
			{
				temp = vertex;
				break;
			}
		}
		vertices.erase(std::find(vertices.begin(), vertices.end(), temp));
		delete temp;
		ConvertVerticesToRawData();
	}

	uint32_t VertexBuffer::CalculateOffset(Vertex* vertex) const
	{
		int offset = 0;
		for (Vertex* vx : vertices)
		{
			if (vx == vertex) break;
			offset += vx->numberOfVertices;
		}
		return offset;
	}

	void VertexBuffer::ConvertVerticesToRawData()
	{
		rawVertices.clear();
		for (Vertex* vertex : vertices)
		{
			rawVertices.insert(rawVertices.end(), vertex->data.begin(), vertex->data.end());
		}

		reloadData = true;
	}

	void VertexBuffer::BufferSubData() const
	{
		if (reloadData)
		{
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * rawVertices.size(), rawVertices.data());
		}
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	//
	// ELEMENT BUFFER
	//

	Shr<ElementBuffer> ElementBuffer::CreateBuffer(size_t size)
	{
		return MakeShr<ElementBuffer>(size);
	}

	ElementBuffer::ElementBuffer(unsigned size)
		:size(size)
	{
		glCreateBuffers(1, &eboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	ElementBuffer::~ElementBuffer()
	{
		for (Element* element : elements)
		{
			delete element;
		}
		glDeleteBuffers(1, &eboID);
	}

	void ElementBuffer::Add(std::vector<unsigned int> data, uint32_t vertexOffset, uint32_t id)
	{
		Element* element = new Element();
		element->data = data;
		element->vertexOffset = vertexOffset;
		element->id = id;
		this->elements.emplace_back(element);
		ConvertElementsToRawData();
	}

	void ElementBuffer::Update(std::vector<unsigned int> data, uint32_t id)
	{
		for (Element* element : elements)
		{
			if (element->id == id)
			{
				element->data = data;
			}
		}
		ConvertElementsToRawData();
	}

	void ElementBuffer::Remove(uint32_t id)
	{
		Element* temp;
		for (Element* element : elements)
		{
			if (element->id == id)
			{
				temp = element;
				break;
			}
		}
		elements.erase(std::find(elements.begin(), elements.end(), temp));
		delete temp;
		ConvertElementsToRawData();
	}

	void ElementBuffer::ConvertElementsToRawData()
	{
		rawElements.clear();
		for (Element* element : elements)
		{
			Element el(*element);
			for (int i = 0; i < el.data.size(); i++)
			{
				element->data[i] += el.vertexOffset;
			}
			rawElements.insert(rawElements.end(), el.data.begin(), el.data.end());
		}
		reloadData = true;
	}

	void ElementBuffer::BufferSubData() const
	{
		if (reloadData)
		{
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(int) * rawElements.size(), rawElements.data());
		}
	}

	void ElementBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	}

	void ElementBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
