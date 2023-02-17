#pragma once
#include "_Core.h"
#include "utility.h"

namespace core {


	struct FrameBufferProperties
	{
		uint32_t width, height;
		uint32_t samples = 1;
	};

	
	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual uint32_t GetColorID() const = 0;

		virtual void Invalidate() = 0;
		virtual void Resize(unsigned int width, unsigned int height) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual FrameBufferProperties& GetProperties() = 0;

		static Shr<FrameBuffer> CreateBuffer(const FrameBufferProperties& properties);
	};
}