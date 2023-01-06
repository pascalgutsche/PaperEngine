#include "_Core.h"

#include "renderer/Texture.h"
#include "generic/Window.h"

#include <GLAD/glad.h>

namespace core {

    Texture::Texture(std::string filePath, bool flipped)
    {
        Texture::filePath = filePath;

        glGenTextures(1, &texID);
        // use texture (everything that is called from now will be set to the current texture)
        glBindTexture(GL_TEXTURE_2D, texID);
        // set texture parameters

        // repeat image in both directions (activate the repeating of the texture)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // pixelate the texture when made bigger
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // pixelate the texture when made smaller
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_set_flip_vertically_on_load(flipped);
        // load texture and save formats to the variables (4 == RGBA format)
        localBuffer = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

        // free memory if path is invalid
        if (localBuffer)
        {
            if (channels == 3) {
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, localBuffer);
            }
            else if (channels == 4) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
            }
            else {
                LOG_CORE_ERROR("Unknown number of channel '" + std::to_string(channels) + "' by texture '" + this->filePath + "'");
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        else {
            LOG_CORE_ERROR("Could not load image '" + this->filePath + "'");
        }

        stbi_image_free(localBuffer);
    }

    Texture::~Texture()
    {
        //delete the texture if it remains unused
        glDeleteTextures(1, &texID);
    }

    void Texture::bind(unsigned int slot)
    {
        // slot is usually set to 0
        if (slot > 8)
        {
            Window::quit("You should not go over 8 slots, as the OpenGL specification does not allow more");
        }
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, texID);
    }

    void Texture::unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    int Texture::getID() {
        return texID;
    }

    int Texture::getWidth()
    {
        return width;
    }

    int Texture::getHeight()
    {
        return height;
    }

    std::string Texture::getFilePath() {
        return this->filePath;
    }

}