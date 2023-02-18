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
        Sprite();
        Sprite(Shr<Texture> texture);
        Sprite(Shr<Texture> texture, float* texCoords);
        virtual ~Sprite();

        void start() override { }
        void stop() override { }
        void update(float dt) override { }
        void event(Event& event) override { }

        Shr<Texture> GetTexture()
        {
            // get current texture
            return this->texture;
        }

        void SetTexture(Shr<Texture> texture) {
            this->texture = texture;
        }

        float* GetTexCoords()
        {
            // get current texture coordinates
            return this->texCoords;
        }

        void SetTexCoords(float* texCoords)
        {
            for (int i = 0; i < 8; i++) {
                this->texCoords[i] = texCoords[i];
            }
        }
    };

}