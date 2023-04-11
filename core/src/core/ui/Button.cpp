#include "_Core.h"
#include "Button.h"


namespace core
{
	Button::Button(glm::vec4 color, const Transform& transform, Type type, std::string name)
		: UIObject(name, transform), color(color), type(type)
	{
	}

	Button::~Button()
	{
	}

	void Button::Render()
	{
		switch (type)
		{
			case Type::Rectangle:

				break;
			case Type::Triangle: 
				break;
		}
	}

}
