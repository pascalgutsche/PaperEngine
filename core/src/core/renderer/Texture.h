#pragma once
#include "_Core.h"

#include <STB_IMAGE/stb_image.h>

namespace core {

    class Texture {
    private:
        std::string filePath;

        unsigned int texID;
        unsigned char* localBuffer;

        int width;
        int height;
        int channels;

    public:
        // load texture with desired filePath
        Texture(std::string filePath, bool flip = 1);
        ~Texture();
        // use texture slot
        void bind(unsigned int slot);

        // stop using texture slot
        void unbind();
        int getID();
        int getWidth();
        int getHeight();
        std::string getFilePath();
    };

}