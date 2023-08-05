#include "Engine.h"
#include "Buffer.h"

namespace Paper
{
	Buffer::Buffer()
		: data(nullptr), size(0)
	{
	}

	Buffer::Buffer(uint32_t size)
	{
		Allocate(size);
	}

	Buffer::Buffer(void* data, uint32_t size)
		: data(data), size(size)
	{
	}

	Buffer::~Buffer()
	{
		Release();
	}

	Buffer Buffer::Copy() const
	{
		Buffer buffer(size);
		memcpy(buffer.data, data, size);
		return buffer;
	}

	void Buffer::CopyTo(Buffer& dst) const
	{
		dst.Allocate(size);
		memcpy(dst.data, data, size);
	}

	void Buffer::CopyFrom(const Buffer& src)
	{
		Allocate(src.size);
		memcpy(data, src.data, src.size);
	}

	void Buffer::Set(void* data, uint32_t size)
	{
		Release();
		this->data = data;
		this->size = size;
	}

	void Buffer::Allocate(uint32_t size)
	{
		Release();
		if (!size)
			return;

		data = new byte[size];
		this->size = size;
	}

	void Buffer::Release()
	{
		delete[](byte*)data;
		size = 0;
		data = nullptr;
	}

	void Buffer::Nullify() const
	{
		memset(data, 0, size);
	}
}
