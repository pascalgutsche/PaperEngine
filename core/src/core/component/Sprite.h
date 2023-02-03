#pragma once
#include "_Core.h"
#include "utility.h"

#include "generic/Component.h"
#include "renderer/Texture.h"

namespace core {

    class Sprite : public Component {
    private:
        std::shared_ptr<Texture> texture;
        float texCoords[8];
    public:
        // get default texture coordinates
        Sprite(Shr<Texture> texture);
        // set sprite texture with specific textureCoordinates
        Sprite(Shr<Texture> texture, float* texCoords);
        virtual ~Sprite();

        void start() override { }
        void stop() override { }
        void update(float dt) override { }

        Shr<Texture> getTexture()
        {
            // get current texture
            return this->texture;
        }

        void setTexture(Shr<Texture> texture) {
            this->texture = texture;
        }

        float* getTexCoords()
        {
            // get current texture coordinates
            return this->texCoords;
        }

        void setTexCoords(float* texCoords)
        {
            for (int i = 0; i < 8; i++) {
                this->texCoords[i] = texCoords[i];
            }
        }
    };

}