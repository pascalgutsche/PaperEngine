#pragma once
#include "_Core.h"
#include "utility.h"

#include "core/renderer/FrameBuffer.h"

namespace core
{
	class OpenGLFramebuffer : public FrameBuffer
	{
	public:
		OpenGLFramebuffer(const FrameBufferProperties& properties);
		~OpenGLFramebuffer() override;

		void Invalidate() override;
		void Resize(unsigned width, unsigned height) override;

		void Bind() override;
		void Unbind() override;

		FrameBufferProperties& GetProperties() override { return frame_buffer_properties; }
		uint32_t GetColorID() const override { return color; }

	private:
		FrameBufferProperties frame_buffer_properties;
		unsigned int fboID = 0;
		unsigned int rbo = 0;
		uint32_t color;
		uint32_t depth;
	};
}
