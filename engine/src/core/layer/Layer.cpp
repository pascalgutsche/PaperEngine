#include "Engine.h"

#include "layer/Layer.h"
#include "event/Event.h"
#include "renderer/Renderer2D.h"

namespace Paper
{
	void Layer::Attach()
	{
		attached = true;
		OnAttach();
	}

	void Layer::Detach()
	{
		attached = false;
		OnDetach();
	}
	
}
