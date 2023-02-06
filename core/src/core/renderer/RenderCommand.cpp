#include "_Core.h"
#include "RenderCommand.h"

namespace core
{
	Shr<RendererAPI> RenderCommand::rendererAPI = RendererAPI::CreateAPI();
}
