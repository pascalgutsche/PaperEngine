#include "_Core.h"
#include "Panel.h"

#include "renderer/Renderer.h"

namespace core
{
	Panel::Panel(glm::vec4 color, const Transform& transform, Type type)
		: UIObject(transform), color(color), type(type) { }

	Panel::~Panel()
	{
	}

	void Panel::Render()
	{
		CalculateGlobalCoords();
		switch (type)
		{
			case Type::Rectangle:
			Renderer::DrawRectangle(globalPos, transform.scale, transform.rotation, color, mode, coreID);
				break;
			case Type::Triangle: 
				break;
			case Type::Circle: 
				break;
		}
	}
}
