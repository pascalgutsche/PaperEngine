#pragma once
#include "Engine.h"
#include "utility.h"

#include "component/2D/Sprite.h"

namespace Paper
{
    class SpriteSheet
    {
    public:
        SpriteSheet(glm::vec4 color, Shr<Texture> texture, float spriteWidth, float spriteHeight, float paddingWidth, float paddingHeight, glm::vec2 selectedSprite, bool registerAlphaPixelsToEvent = false);

        void Init(Shr<Texture> texture, glm::vec2 selectedSprite);

        //void OnStart() override { }
        //void OnStop() override { }
        //void OnUpdate() override { };
        //void OnImgui(float dt) override { };
        //void OnEvent(Event& event) override { }
        //void OnRender() override;

        int GetSpriteCount() const { return spriteRows + spriteColumns; };
        int GetSpriteRows() const { return spriteRows; };
        int GetSpriteColumns() const { return spriteColumns; };

        void ChangeSprite(glm::vec2 pos);


    private:
        glm::vec4 color;
        Shr<Texture> texture;

        std::array<glm::vec2, 4> texCoords;

        float spriteWidth;
        float spriteHeight;

        float paddingWidth;
        float paddingHeight;

        float textureWidth;
        float textureHeight;

        int spriteRows;
        int spriteColumns;

        glm::vec2 selectedSprite;

        bool registerAlphaPixelsToEvent;

        std::vector<std::vector<Shr<Sprite>>> Sprites;

        void ChangeCoords();
    };
};