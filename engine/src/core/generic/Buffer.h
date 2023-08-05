#pragma once


namespace Paper
{
	class Buffer
	{
		using byte = uint8_t;
	public:
		void* data;
		uint32_t size;

		Buffer();
		Buffer(uint32_t size);
		Buffer(void* data, uint32_t size);

		~Buffer();

		Buffer Copy() const;
		void CopyTo(Buffer& dst) const;
		void CopyFrom(const Buffer& src);

		void Set(void* data, uint32_t size);

		void Allocate(uint32_t size);
		void Release();

		void Nullify() const;

		template<typename T>
		T& Read()
		{
			return *(T*)data;
		}

		template<typename T>
		T* As()
		{
			return (T*)data;
		}
	};
}

