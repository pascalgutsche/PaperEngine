#pragma once
#include "_Core.h"
#include "utility.h"

#include <STB_IMAGE/stb_image.h>

namespace core {

    class CORE_API Texture {
    private:
        std::string filePath;
        std::string name;

        unsigned char* localBuffer;

        int width;
        int height;
        int channels;

        unsigned int texID;

        bool init(std::string path);

    public:
        // load texture with desired filePath
        Texture(std::string filePath, std::string name);
        ~Texture();
        // use texture slot
        void bind(unsigned int slot);
        


        // stop using texture slot
        void unbind();
        int getID();
        int getWidth();
        int getHeight();
        std::string getFilePath();
        std::string getName();
    };

}