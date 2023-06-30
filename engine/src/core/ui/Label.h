#pragma once
#include "Engine.h"

#include "UIObject.h"

namespace ppr
{
	class Label : public UIObject
	{
	public:
		Label(std::string text, glm::vec4 color, const Transform& transform, const Shr<Font>& font, std::string name = "Label");
		~Label() override;

		void Render(entity_id idToRender) override;

		std::string text;
	private:
		Shr<Font> font;
	};
}

