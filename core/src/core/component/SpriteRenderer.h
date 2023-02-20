#pragma once
#include "_Core.h"
#include "utility.h"

#include "generic/Component.h"
#include "generic/Transform.h"
#include "component/Sprite.h"

namespace core {

    struct RenderData;

    class SpriteRenderer : public Component {
    private:
        glm::vec4 color;
        Sprite* sprite;
        Transform* lastTransform = nullptr;
        RenderData* renderData = nullptr;
    public:
        SpriteRenderer(glm::vec4 color, Shr<Texture> texture = nullptr);
        SpriteRenderer(glm::vec4 color, Sprite* sprite);

        virtual ~SpriteRenderer();

        void start() override;
        void stop() override;
        void Update() override;
        void imgui(float dt) override;
        void event(Event& event) override { }

        //void UpdateTexture(Shr<Texture> texture);

        

        glm::vec4 GetColor() {
            // return current sprite color
            return this->color;
        }

        Shr<Texture> GetTexture()
        {
            // return current texture that is being used in this sprite
            return sprite->GetTexture();
        }

        float* GetTexCoords()
        {
            // return the coordinates of the texture that is being used in this sprite
            return this->sprite->GetTexCoords();
        }

        void SetSprite(Sprite* sprite)
        {
            // set the sprite to the function parameter desired sprite
            this->sprite = sprite;
        }

    };

}