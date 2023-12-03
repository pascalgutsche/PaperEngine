#include "Engine.h"
#include "utility.h"
#include "RenderAPI.h"

#include "OpenGL/OpenGLRenderAPI.h"

namespace Paper
{
	RenderAPI::API RenderAPI::api = OPENGL;

	Ref<RenderAPI> RenderAPI::CreateAPI()
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::NONE: CORE_ASSERT(false, "'NONE' is a non valid API"); return nullptr;
		case RenderAPI::OPENGL: return MakeShr<OpenGLRenderAPI>();
		case RenderAPI::VULKAN: CORE_ASSERT(false, "'VULKAN' is currently a not supportet API"); return nullptr;;
		}

		CORE_ASSERT(false, "");
		return nullptr;
	}


	

}
