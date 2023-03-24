#include "_Core.h"
#include "component/SpriteSheet.h"
#include "renderer/Renderer.h"

namespace core
{	
	SpriteSheet::SpriteSheet(glm::vec4 color, Shr<Texture> texture, float spriteWidth, float spriteHeight, glm::vec2 selectedSprite)
		:color(color), texture(texture), spriteWidth(spriteWidth), spriteHeight(spriteHeight), selectedSprite(selectedSprite)
	{
		Init(color, texture, spriteWidth, spriteHeight, selectedSprite);
	}

	void SpriteSheet::Init(glm::vec4 color, Shr<Texture> texture, float spriteWidth, float spriteHeight, glm::vec2 selectedSprite)
	{
		this->textureWidth = texture->GetWidth();
		this->textureHeight = texture->GetHeight();
		this->spriteColumns = textureWidth / this->spriteWidth;
		this->spriteRows = textureHeight / this->spriteHeight;
		this->selectedSprite = selectedSprite;

		ChangeSprite(selectedSprite);
	}

	void SpriteSheet::ChangeSprite(glm::vec2 pos)
	{
		selectedSprite = pos;
		ChangeHelper();
	}

	void SpriteSheet::ChangeHelper()
	{
		this->texCoords[0] = { (spriteWidth * selectedSprite.x)       / textureWidth, (spriteHeight * selectedSprite.y)       / textureHeight };
		this->texCoords[1] = { (spriteWidth * (selectedSprite.x + 1)) / textureWidth, (spriteHeight * selectedSprite.y)       / textureHeight };
		this->texCoords[2] = { (spriteWidth * (selectedSprite.x + 1)) / textureWidth, (spriteHeight * (selectedSprite.y + 1)) / textureHeight };
		this->texCoords[3] = { (spriteWidth * selectedSprite.x)       / textureWidth, (spriteHeight * (selectedSprite.y + 1)) / textureHeight };
	}
	void SpriteSheet::OnUpdate()
	{
		Renderer::DrawRectangle(gameObject->transform.position, gameObject->transform.scale, gameObject->transform.rotation, texCoords, texture, 1.0f, color, gameObject->GetProjectionMode(), gameObject->GetObjectID());
	}

	void SpriteSheet::OnImgui(float dt)
	{

	}

};