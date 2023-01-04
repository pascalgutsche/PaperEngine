#include "_Core.h"

#include "utils/DataPool.h"
#include "renderer/Texture.h"
#include "generic/Shader.h"

namespace core {

    // freetype developers are crazy
    //#include "../lib/freetype/ft2build.h"
    //#include FT_FREETYPE_H

    // declare static maps in order to find a reference
    std::unordered_map<std::string, std::shared_ptr<Shader>> DataPool::shaderPool;
    std::unordered_map<std::string, Texture*> DataPool::texturePool;

    //struct CharacterTemplateS CharacterTemplate;

    //std::map<char, CharacterTemplateS> DataPool::CharacterPool;

    std::shared_ptr<Shader> DataPool::getShader(std::string shaderName) {
        // we need a shared pointer because why not
        // set default path
        std::string path = "assets/shaders/";

        // search the map for shader string and return the shader if it exists
        if (shaderPool.find(shaderName) != shaderPool.end()) {
            return shaderPool.at(shaderName);
        }
        // create the shader either way
        shaderPool[shaderName] = std::make_shared<Shader>(path + shaderName + ".glsl");
        // return it, if it did not exist
        return shaderPool.at(shaderName);
    }

    Texture* DataPool::getTexture(std::string textureName, bool flipped) {
        // set default path
        std::string path = "assets/textures/";
        // search for texture with texture name and return it if it exists
        if (flipped) {
            if (texturePool.find(textureName) != texturePool.end()) {
                return texturePool.at(textureName);
            }
            // create the texture and return it
            texturePool[textureName] = new Texture(path + textureName);
            return texturePool.at(textureName);
        }
        else {
            if (texturePool.find(textureName + "_notflipped") != texturePool.end()) {
                return texturePool.at(textureName + "_notflipped");
            }
            // create the texture and return it
            texturePool[textureName + "_notflipped"] = new Texture(path + textureName, false);
            return texturePool.at(textureName + "_notflipped");
        }
    }

    // basically save our characters we need for rendering into the CharacterPool map
    // each character and it's mandatory values for rendering (advance,... (struct variables)) are
    // saved into a map, which can then be used later on
    // create the texture once, use it anytime else
    // this function should be called at an early stage
    // also, call this function before you would want to use the CharacterPool map...
    /*void DataPool::prepareCharacters()
    {
        // firstly, we need to create freetype's objects in order to use their functions
        FT_Library ftl;

        // typical initializing

        // functions return non-zero on error
        if(FT_Init_FreeType(&ftl))
        {
            Logger::Log("Could not load FreeType Library", Logger::Error);
            return;
        }

        FT_Face face;

        if(FT_New_Face(ftl, "assets/fonts/mononoki.ttf", 0, &face))
        {
            Logger::Log("Could not load font face", Logger::Error);
            return;
        }

        // setting the width to 0, makes the library dynamically adjust based to it's height
        FT_Set_Pixel_Sizes(face, 0, 48);

        // needed in order to store the color of a glyph in one byte
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // iterating through the ascii table
        for(uint8_t i = 0; i < 256; i++)
        {
            // load the character
            if(FT_Load_Char(face, i, FT_LOAD_RENDER))
            {
                Logger::Log("Could not load character", Logger::Error);
            }

            unsigned int textureID;

            // generate texture (space) and save the location of the texture into textureID
            glGenTextures(1, &textureID);

            // bind the texture and specify it's dimension
            // this is needed for specifying the character's sizes
            glBindTexture(GL_TEXTURE_2D, textureID);

            // fill the texture with values of the character
            // the character texture has only one color, as we can limit the color usage to 1 byte
            // one could use any color, but one
            glTexImage2D(GL_TEXTURE_2D, 0, GL_BLUE,
                        face->glyph->bitmap.width, face->glyph->bitmap.rows,
                        0, GL_BLUE, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

            // texture settings
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // don't repeat left
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // don't repeat right
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // useful for resizing bigger
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // useful for resizing smaller

            // save the pre-rendered character texture values into the struct
            // then save it into the map, ordered by char
            CharacterTemplateS CharacterTemplate =
            {
                textureID,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            // saving into map
            CharacterPool.insert(std::pair<char, CharacterTemplateS>(i, CharacterTemplate));

            //TODO: fix undefined references
            // argh
        }*/

}
