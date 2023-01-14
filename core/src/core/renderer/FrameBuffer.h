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
	private:
		FrameBufferProperties frame_buffer_properties;
		unsigned int fboID = 0;
		unsigned int rbo = 0;
		uint32_t color;
		uint32_t depth;
	public:
		FrameBuffer(const FrameBufferProperties& properties);
		~FrameBuffer();

		uint32_t GetColorID() const { return color; }

		void Invalidate();
		void Resize(unsigned int width, unsigned int height);

		void Bind();
		void Unbind();

		FrameBufferProperties& GetProperties() { return frame_buffer_properties; }
	};
}


