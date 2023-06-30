#pragma once
#include "Engine.h"

#include "UIObject.h"

namespace ppr
{
	class Panel : public UIObject
	{
	public:
		Panel(glm::vec4 color, const Transform& transform, Type type, std::string name = "Panel");
		~Panel() override;

		void Render(entity_id idToRender) override;

	private:
		Type type;
	};
}

