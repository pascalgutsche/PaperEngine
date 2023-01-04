#pragma once
#include "_Core.h"

#include "generic/Shader.h"
#include "renderer/Texture.h"

namespace core {

    /*struct CharacterTemplateS
    {
        unsigned int textureID; // texture ID, used by opengl
        glm::ivec2 size; // size of character
        glm::ivec2 bearing; // height offset
        unsigned int advance; // width offset
    };*/

    class DataPool {
    private:
        // map in order to save all of the shaders somewhere for organization
        static std::unordered_map<std::string, std::shared_ptr<Shader>> shaderPool;
        // map for all of the textures in a texturePool
        static std::unordered_map<std::string, Texture*> texturePool;

    public:
        // get current shader with the specific string (name)
        static std::shared_ptr<Shader> getShader(std::string shaderName);
        // search for a texture with the specific string (name)
        static Texture* getTexture(std::string textureName, bool flipped = true);

        //static std::map<char, CharacterTemplateS> CharacterPool;

        //static void prepareCharacters();
        enum DISPLAYMODE { NONE, PERSPECTIVE, ORTHOGRAPHIC };
    };

}