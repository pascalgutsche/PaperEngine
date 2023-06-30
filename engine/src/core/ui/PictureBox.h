#pragma once
#include "Engine.h"
#include "utility.h"

#include "UIObject.h"

namespace ppr
{
	class PictureBox : public UIObject
	{
	public:
		PictureBox(glm::vec4 color, const Transform& transform, Shr<Texture> texture, Type type, std::string name = "PictureBox");
		PictureBox(glm::vec4 color, const Transform& transform, Shr<Texture> texture, float tilingFactor, Type type, std::string name = "Panel");
		~PictureBox() override;
		void Render(entity_id idToRender) override;

	private:
		Shr<Texture> texture;
		float tilingFactor;
		Type type;
	};
	
}
