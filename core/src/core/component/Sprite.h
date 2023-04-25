#pragma once
#include "_Core.h"

#include "renderer/Texture.h"

namespace core
{
	class Sprite
	{
	public:
		Sprite(uint32_t spriteWidth, uint32_t spriteHeight, uint32_t paddingWidth, uint32_t paddingHeight, uint32_t selectedSprite);
		~Sprite();

		void SetColor(glm::vec4 color);
		void SetTexture(Shr<Texture> texture);
		void SetTexCoords(std::array<glm::vec2, 4> texCoords);
		void SetTilingFactor();

		glm::vec4 GetColor() const;
		Shr<Texture> GetTexture() const;
		std::array<glm::vec2, 4> GetTexCoords() const;
		float GetTilingFactor() const;

	};
}

