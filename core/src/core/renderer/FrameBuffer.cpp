#include "_Core.h"
#include "FrameBuffer.h"

#include <glad/glad.h>

namespace core {

	FrameBuffer::FrameBuffer() {

	}

	FrameBuffer::~FrameBuffer() {
		glDeleteFramebuffers(1, &fboID);
		//glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		//glDeleteTextures(1, &m_DepthAttachment);
	}

	void FrameBuffer::invalidate() {
		if (fboID)
		{
			glDeleteFramebuffers(1, &fboID);
			//glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			//glDeleteTextures(1, &m_DepthAttachment);

			//m_ColorAttachments.clear();
			//m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &fboID);
		glBindFramebuffer(GL_FRAMEBUFFER, fboID);
	}

	void FrameBuffer::bind() {

	}

	void FrameBuffer::unbind() {

	}

}