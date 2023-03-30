#include "_Core.h"
#include "utility.h"
#include "renderer/Texture2D.h"

#include "OpenGL/OpenGLTexture2D.h"

#include "renderer/RenderAPI.h"

namespace core {

	Shr<Texture2D> Texture2D::CreateTexture(TextureSpecification specification)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::NONE: CORE_ASSERT(false, "'NONE' is a non valid API"); return nullptr;
		case RenderAPI::OPENGL: return MakeShr<OpenGLTexture2D>(specification);
		case RenderAPI::VULKAN: CORE_ASSERT(false, "'VULKAN' is currently a not supportet API"); return nullptr;
		}

		CORE_ASSERT(false, "");
		return nullptr;
	}

	Shr<Texture2D> Texture2D::CreateTexture(std::string filePath, std::string name)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::NONE: CORE_ASSERT(false, "'NONE' is a non valid API"); return nullptr;
		case RenderAPI::OPENGL: return MakeShr<OpenGLTexture2D>(filePath, name);
		case RenderAPI::VULKAN: CORE_ASSERT(false, "'VULKAN' is currently a not supportet API"); return nullptr;
		}

		CORE_ASSERT(false, "");
		return nullptr;
	}

}