#include "Engine.h"
#include "Panel.h"

#include "renderer/Renderer2D.h"

namespace ppr
{
	Panel::Panel(glm::vec4 color, const Transform& transform, Type type, std::string name)
		: UIObject(name, transform, color), type(type) { }

	Panel::~Panel()
	{
	}

	void Panel::Render(entity_id idToRender)
	{
		//EdgeRenderData data;
		//data.transform = { globalPos, globalScale, transform.rotation };
		//data.color = color;
		//data.mode = mode;
		//data.coreID = idToRender;
		//
		//switch (type)
		//{
		//	case Type::Rectangle:
		//		Renderer2D::DrawRectangle(data);
		//		break;
		//	case Type::Triangle:
		//		Renderer2D::DrawTriangle(data);
		//		break;
		//}
	}
}
