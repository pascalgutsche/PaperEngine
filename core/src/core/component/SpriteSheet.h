#pragma once
#include "_Core.h"
#include "utility.h"

#include "generic/Component.h"
#include "component/Sprite.h"

namespace core
{
	class SpriteSheet : public Component
	{
	public:
        SpriteSheet(glm::vec4 color, Shr<Texture> texture, uint32_t spriteWidth, uint32_t spriteHeight, uint32_t paddingWidth, uint32_t paddingHeight, uint32_t selectedSprite, bool registerAlphaPixelsToEvent = false);

        void Init(Shr<Texture> texture, uint32_t selectedSprite);

        void OnStart() override { }
        void OnStop() override { }
        void OnUpdate() override;
        void OnImgui(float dt) override;
        void OnEvent(Event& event) override { }

        int GetSpriteCount() { return spriteRows + spriteColumns; };
        int GetSpriteRows() { return spriteRows; };
        int GetSpriteColumns() { return spriteColumns; };

        void ChangeSprite(uint32_t index);


    private:
        glm::vec4 color;
        Shr<Texture> texture;

        glm::vec2 texCoords[4];

        uint32_t spriteWidth;
        uint32_t spriteHeight;

        uint32_t paddingWidth;
        uint32_t paddingHeight;

        uint32_t textureWidth;
        uint32_t textureHeight;

        uint32_t spriteRows;
        uint32_t spriteColumns;

        glm::vec2 selectedSprite;

        bool registerAlphaPixelsToEvent;

		void ChangeCoords();
	};
};