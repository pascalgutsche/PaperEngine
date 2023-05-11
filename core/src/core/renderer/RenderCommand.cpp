#include "_Core.h"
#include "RenderCommand.h"

#include "generic/Application.h"

namespace core
{
	Shr<RenderAPI> RenderCommand::rendererAPI = RenderAPI::CreateAPI();
	Shr<Framebuffer> RenderCommand::framebuffer;

	void RenderCommand::Init()
	{
		rendererAPI->Init();
		FramebufferSpecification spec;
		spec.attachment = { FramebufferTexFormat::RGBA8, FramebufferTexFormat::RED_INTEGER, FramebufferTexFormat::Depth };
		spec.width = Application::GetWindow()->GetWidth();
		spec.height = Application::GetWindow()->GetHeight();
		framebuffer = Framebuffer::CreateBuffer(spec);
	}

	void RenderCommand::Clear()
	{
		rendererAPI->Clear();
	}

	void RenderCommand::PrepareFramebuffer()
	{
		framebuffer->Bind();
		Clear();
		framebuffer->ClearAttachment(1, 0);
		framebuffer->Unbind();
	}

	void RenderCommand::EnableDepthTesting(bool enable)
	{
		return rendererAPI->EnableDepthTesting(enable);
	}

	bool RenderCommand::IsDepthTestingEnabled()
	{
		return rendererAPI->IsDepthTestingEnabled();
	}
}
