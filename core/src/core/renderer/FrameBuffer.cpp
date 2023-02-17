#include "_Core.h"
#include "FrameBuffer.h"

#include "RenderAPI.h"

#include "OpenGL/OpenGLFramebuffer.h"

namespace core {

	Shr<FrameBuffer> FrameBuffer::CreateBuffer(const FrameBufferProperties& properties)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::NONE: CORE_ASSERT(false, "'NONE' is a non valid API"); return nullptr;
		case RenderAPI::OPENGL: return MakeShr<OpenGLFramebuffer>(properties);
		case RenderAPI::VULKAN: CORE_ASSERT(false, "'VULKAN' is currently a not supportet API"); return nullptr;;
		}

		CORE_ASSERT(false, "");
		return nullptr;
	}

}