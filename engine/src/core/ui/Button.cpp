#include "Engine.h"
#include "Button.h"

#include "renderer/Renderer2D.h"


namespace engine
{
	Button::Button(glm::vec4 color, const Transform& transform, Type type, const ButtonEventCallbackFunction& buttonEventFunction, std::string name)
		: UIObject(name, transform, color), type(type), buttonEventFunction(buttonEventFunction)
	{
		buttonID = coreID;
	}

	Button::~Button()
	{
	}

	void Button::Render(core_id idToRender)
	{
		EdgeRenderData data;
		data.transform = { globalPos, globalScale, transform.rotation };
		data.color = color;
		data.mode = mode;
		data.coreID = idToRender;

		switch (type)
		{
			case Type::Rectangle:
				Renderer2D::DrawRectangle(data);
				break;
			case Type::Triangle:
				Renderer2D::DrawTriangle(data);
				break;
		}
	}

}
