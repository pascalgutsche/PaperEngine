#pragma once
#include "_Core.h"
#include "utility.h"

#include "generic/Component.h"
#include "generic/Transform.h"
#include "component/Sprite.h"

namespace core {

    class CORE_API SpriteRenderer : public Component {
    private:
        glm::vec4 color;
        Sprite* sprite;
        Transform* lastTransform;

        bool isDirty = false;
    public:
        SpriteRenderer(glm::vec4 color);
        SpriteRenderer(Sprite* sprite);

        virtual ~SpriteRenderer();

        /* troll */
        // setup function
        void start() override;
        // update function call on every update (frame)
        void update(float dt) override;
        void imgui(float dt) override;
        glm::vec4 getColor();
        Texture* getTexture();
        float* getTexCoords();

        void setSprite(Sprite* sprite);
        void setColor(glm::vec4 color);

        bool getIsDirty();
        void setClean();

    };

}