#include "Engine.h"
#include "Label.h"

#include "renderer/Renderer2D.h"


namespace ppr
{
	Label::Label(std::string text, glm::vec4 color, const Transform& transform, const Shr<Font>& font, std::string name)
		: UIObject(name, transform, color), text(text), font(font)
	{
	}

	Label::~Label()
	{
	}

	void Label::Render(entity_id idToRender)
	{
		//TextRenderData data;
		//data.transform = { globalPos, globalScale, transform.rotation };
		//data.color = color;
		//data.string = text;
		//data.font = font;
		//data.mode = mode;
		//data.coreID = idToRender;
		//Renderer2D::DrawString(data);
	}

}
