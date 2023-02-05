#pragma once
#include "_Core.h"
#include "utility.h"

namespace  core
{
	enum class GLSLDataType
	{
		NONE,
		FLOAT, FLOAT2, FLOAT3, FLOAT4,
		INT, INT2, INT3, INT4,
		MAT3, MAT4,
		BOOL
	};

	static int GetDataTypeSizeBytes(GLSLDataType type)
	{
		switch (type)
		{
		case GLSLDataType::FLOAT:   return 4 * 1;
		case GLSLDataType::FLOAT2:  return 4 * 2;
		case GLSLDataType::FLOAT3:  return 4 * 3;
		case GLSLDataType::FLOAT4:  return 4 * 4;
		case GLSLDataType::INT:		return 4 * 1;
		case GLSLDataType::INT2:	return 4 * 2;
		case GLSLDataType::INT3:	return 4 * 3;
		case GLSLDataType::INT4:	return 4 * 4;
		case GLSLDataType::MAT3:	return 4 * 3 * 3;
		case GLSLDataType::MAT4:	return 4 * 4 * 4;
		case GLSLDataType::BOOL:	return 1;
		default:;
		}

		CORE_ASSERT(false, "unknown GLSLDataType");
		return 0;
	}

	struct BufferElement
	{
		std::string name;
		GLSLDataType type;
		uint32_t size;
		uint32_t count;
		size_t offset;
		bool normalized;

		BufferElement() = default;

		BufferElement(GLSLDataType type, std::string name, bool normalized = false)
			: name(name), type(type), size(GetDataTypeSizeBytes(type)), count(GetDataTypeCount()), offset(0), normalized(normalized) { }

	private:

		int GetDataTypeCount() const
		{
			switch (type)
			{
			case GLSLDataType::FLOAT:   return 1;
			case GLSLDataType::FLOAT2:  return 2;
			case GLSLDataType::FLOAT3:  return 3;
			case GLSLDataType::FLOAT4:  return 4;
			case GLSLDataType::INT:		return 1;
			case GLSLDataType::INT2:	return 2;
			case GLSLDataType::INT3:	return 3;
			case GLSLDataType::INT4:	return 4;
			case GLSLDataType::MAT3:	return 3;
			case GLSLDataType::MAT4:	return 4;
			case GLSLDataType::BOOL:	return 1;
			default:;
			}

			CORE_ASSERT(false, "unknown GLSLDataType");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			:elements(elements)
		{
			CalculateData();
		}

		uint32_t GetStride() const { return stride; }
		uint32_t GetSize() const { return size; }
		std::vector<BufferElement> GetElements() const { return elements; }

		std::vector<BufferElement>::iterator begin() { return elements.begin(); }
		std::vector<BufferElement>::iterator end() { return elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return elements.end(); }

	private:
		std::vector<BufferElement> elements;

		uint32_t stride = 0;
		uint32_t size = 0;
		void CalculateData()
		{
			size_t offset = 0;
			stride = 0;
			size = 0;
			for (auto& element : elements)
			{
				element.offset = offset;
				offset += element.size;
				stride += element.size;
				size += element.count;
			}
		}

		
	};

	struct Vertex
	{
		std::vector<float> data;

		uint32_t numberOfVertices;
		uint32_t vertexOffset;

		uint32_t id;
	};
	
	class VertexBuffer
	{
	public:
		void Bind() const;
		void Unbind() const;

		uint32_t Add(std::vector<float> data, uint32_t id);
		void Update(std::vector<float> data, uint32_t id);
		void Remove(uint32_t id);

		void BufferSubData() const;

		const BufferLayout& GetLayout() const
		{
			return this->layout;
		}

		static Shr<VertexBuffer> CreateBuffer(BufferLayout layout, unsigned int size);

		VertexBuffer(BufferLayout layout, unsigned int size);
		~VertexBuffer();
	private:
		size_t size;
		BufferLayout layout;

		std::vector<Vertex*> vertices;
		std::vector<float> rawVertices;
		bool reloadData = false;

		uint32_t vboID;

		uint32_t CalculateOffset(Vertex* vertex) const;
		void ConvertVerticesToRawData();
	};

	struct Element
	{
		std::vector<unsigned int> data;

		uint32_t vertexOffset;

		uint32_t id;
	};

	class ElementBuffer
	{
	public:
		void Bind() const;
		void Unbind() const;

		void Add(std::vector<unsigned int> data, uint32_t vertexOffset, uint32_t id);
		void Update(std::vector<unsigned int> data, uint32_t id);
		void Remove(uint32_t id);

		void BufferSubData() const;

		unsigned int GetElementCount() const { return rawElements.size(); }

		static Shr<ElementBuffer> CreateBuffer(size_t size);

		ElementBuffer(unsigned int size);
		~ElementBuffer();
	private:
		size_t size;

		std::vector<Element*> elements;
		std::vector<unsigned int> rawElements;
		bool reloadData = false;

		uint32_t eboID;


		void ConvertElementsToRawData();
	};
}
