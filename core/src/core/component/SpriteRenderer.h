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
        Transform* lastTransform;
        RenderData* renderData;
    public:
        SpriteRenderer(glm::vec4 color, Shr<Texture> texture = nullptr);
        SpriteRenderer(glm::vec4 color, Sprite* sprite);

        virtual ~SpriteRenderer();

        void start() override;
        void stop() override;
        // update function call on every update (frame)
        void update(float dt) override;
        void imgui(float dt) override;

        void UpdateColor(glm::vec4 color);
        void UpdateTexture(Shr<Texture> texture);

        

        glm::vec4 getColor() {
            // return current sprite color
            return this->color;
        }

        Shr<Texture> getTexture()
        {
            // return current texture that is being used in this sprite
            return sprite->getTexture();
        }

        float* getTexCoords()
        {
            // return the coordinates of the texture that is being used in this sprite
            return this->sprite->getTexCoords();
        }

        void setSprite(Sprite* sprite)
        {
            // set the sprite to the function parameter desired sprite
            this->sprite = sprite;
        }

    };

}