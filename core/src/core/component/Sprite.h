#pragma once
#include "_Core.h"
#include "utility.h"

#include "generic/Component.h"
#include "renderer/Texture.h"

namespace core {

    class CORE_API Sprite : public Component {
    private:
        Texture* texture;
        float texCoords[8];
    public:
        // get default texture coordinates
        Sprite(Texture* texture);
        // set sprite texture with specific textureCoordinates
        Sprite(Texture* texture, float* texCoords);
        void setTexture(Texture* texture);
        Texture* getTexture();

        float* getTexCoords();
    };

}