#pragma once
#include "_Core.h"

#include "generic/GameObject_Component.h"
#include "generic/Transform.h"
#include "Sprite.h"

namespace core {

    class SpriteRenderer : public Component {
    private:
        glm::vec4 color;
        Sprite* sprite;
        Transform* lastTransform;

        bool isDirty = false;
    public:
        SpriteRenderer(glm::vec4 color);
        SpriteRenderer(Sprite* sprite);

        ~SpriteRenderer();

        /* troll */
        // setup function
        void start() override;
        // update function call on every update (frame)
        void update(float deltaTime) override;
        void imgui(float deltaTime) override;
        glm::vec4 getColor();
        Texture* getTexture();
        float* getTexCoords();

        void setSprite(Sprite* sprite);
        void setColor(glm::vec4 color);

        bool getIsDirty();
        void setClean();

    };

}