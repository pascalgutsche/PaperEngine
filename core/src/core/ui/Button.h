#pragma once
#include "_Core.h"

#include "UIObject.h"

namespace core
{
	class Button : public UIObject
	{
	public:
		Button(glm::vec4 color, const Transform& transform, Type type, std::string name = "Button");
		~Button() override;

		void Render() override;

	private:
		glm::vec4 color;
		Type type;
	};
}

