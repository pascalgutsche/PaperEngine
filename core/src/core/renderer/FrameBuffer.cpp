#include "_Core.h"
#include "FrameBuffer.h"

#include <glad/glad.h>

namespace core {

	FrameBuffer::FrameBuffer(const FrameBufferProperties& properties)
		:frame_buffer_properties(properties)
	{
		Invalidate();
	}

	FrameBuffer::~FrameBuffer() {
		glDeleteFramebuffers(1, &fboID);
		glDeleteTextures(1, &color);
		glDeleteTextures(1, &depth);
	}

	void FrameBuffer::Invalidate() {

		if (fboID)
		{
			glDeleteFramebuffers(1, &fboID);
			glDeleteTextures(1, &color);
			glDeleteTextures(1, &depth);
		}

		glCreateFramebuffers(1, &fboID);
		glBindFramebuffer(GL_FRAMEBUFFER, fboID);

		//color
		glCreateTextures(GL_TEXTURE_2D, 1, &color);
		glBindTexture(GL_TEXTURE_2D, color);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, frame_buffer_properties.width, frame_buffer_properties.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);
		
		//depth
		glCreateTextures(GL_TEXTURE_2D, 1, &depth);
		glBindTexture(GL_TEXTURE_2D, depth);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, frame_buffer_properties.width, frame_buffer_properties.height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

		CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Resize(unsigned width, unsigned height)
	{
		frame_buffer_properties.width = width;
		frame_buffer_properties.height = height;

		Invalidate();
	}

	void FrameBuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, fboID);
	}

	void FrameBuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}