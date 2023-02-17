#include "_Core.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace core
{
	//
	// VERTEX BUFFER
	//

	OpenGLVertexBuffer::OpenGLVertexBuffer(BufferLayout& layout, unsigned size)
		: size(size), layout(layout)
	{
		glCreateBuffers(1, &vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		for (Vertex* vertex : vertices)
		{
			delete vertex;
		}
		glDeleteBuffers(1, &vboID);
	}

	

	void OpenGLVertexBuffer::Add(std::vector<float> data, uint32_t id)
	{
		Vertex* vertex = new Vertex;
		vertex->data = data;
		vertex->id = id;
		this->vertices.emplace_back(vertex);
		ConvertVerticesToRawData();
	}

	void OpenGLVertexBuffer::Update(std::vector<float> data, uint32_t id)
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

	void OpenGLVertexBuffer::Remove(uint32_t id)
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

	void OpenGLVertexBuffer::ConvertVerticesToRawData()
	{
		rawVertices.clear();
		for (Vertex* vertex : vertices)
		{
			rawVertices.insert(rawVertices.end(), vertex->data.begin(), vertex->data.end());
		}

		reloadData = true;
	}

	void OpenGLVertexBuffer::BufferSubData()
	{
		if (reloadData)
		{
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * rawVertices.size(), rawVertices.data());
		}
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

	OpenGLElementBuffer::OpenGLElementBuffer(size_t size)
		:size(size)
	{
		glCreateBuffers(1, &eboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLElementBuffer::~OpenGLElementBuffer()
	{
		for (Element* element : elements)
		{
			delete element;
		}
		glDeleteBuffers(1, &eboID);
	}

	void OpenGLElementBuffer::Add(std::vector<unsigned int> data, uint32_t id)
	{
		Element* element = new Element();
		element->data = data;
		element->id = id;
		this->elements.emplace_back(element);
		ConvertElementsToRawData();
	}

	void OpenGLElementBuffer::Update(std::vector<unsigned int> data, uint32_t id)
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

	void OpenGLElementBuffer::Remove(uint32_t id)
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

	void OpenGLElementBuffer::ConvertElementsToRawData()
	{
		rawElements.clear();
		int highestElement = 0;
		for (Element* element : elements)
		{
			unsigned int temp = 0;
			Element el(*element);
			for (int i = 0; i < el.data.size(); i++)
			{
				if (el.data[i] > temp)
				{
					temp = el.data[i];
				}
				el.data[i] += highestElement;
			}
			rawElements.insert(rawElements.end(), el.data.begin(), el.data.end());
			temp++;
			highestElement += temp;
		}
		reloadData = true;
	}

	void OpenGLElementBuffer::BufferSubData() 
	{
		if (reloadData)
		{
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(int) * rawElements.size(), rawElements.data());
		}
	}

	unsigned OpenGLElementBuffer::GetElementCount()
	{
		return this->rawElements.size();
	}

	void OpenGLElementBuffer::Bind() 
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	}

	void OpenGLElementBuffer::Unbind() 
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}