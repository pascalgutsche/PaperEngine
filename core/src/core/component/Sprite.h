#pragma once
#include "_Core.h"
#include "utility.h"

#include "generic/Component.h"
#include "renderer/Texture.h"

namespace core {

    class CORE_API Sprite : public Component {
    private:
        std::shared_ptr<Texture> texture;
        float texCoords[8];
    public:
        // get default texture coordinates
        Sprite(std::shared_ptr<Texture> texture);
        // set sprite texture with specific textureCoordinates
        Sprite(std::shared_ptr<Texture> texture, float* texCoords);
        virtual ~Sprite();
        void setTexture(std::shared_ptr<Texture> texture);
        std::shared_ptr<Texture> getTexture();

        float* getTexCoords();
    };

}