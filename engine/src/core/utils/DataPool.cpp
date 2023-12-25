#include "Engine.h"

#include "utils/DataPool.h"

#include "project/Project.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "renderer/Font.h"

namespace Paper {

    // declare static maps in order to find a reference
    Map DataPool::dataPool;

    Ref<Shader> DataPool::GetShader(std::string shaderName) {
        // set default path
        std::string path = "resources/shaders/" + shaderName + ".glsl";
        std::string shader_id = "shader_" + shaderName;

        Map::iterator it = dataPool.find(shader_id);

        std::shared_ptr<Shader> shader;

        if (it == dataPool.end())
        {
            shader = Shader::CreateShader(path);
            dataPool.emplace(shader_id, shader);
        }
        else
        {
            shader = std::static_pointer_cast<Shader>(it->second);
        }

        return shader;
    }

    //Shr<Texture> DataPool::GetAssetTexture(std::string textureName, bool wholePath) {
    //    // set default path
    //    std::filesystem::path texturePath;
    //    if (wholePath)
    //        texturePath = Project::GetProjectPath() / (Project::GetRelativePathFromProject(textureName));
    //    else
    //        texturePath = Project::GetAssetTexturesPath() / (Project::GetRelativePathFromProject(textureName));
    //
    //    std::string texture_id = "assettexture_" + textureName;
    //
    //    Map::iterator it = dataPool.find(texture_id);
    //
    //    std::shared_ptr<Texture> texture;
    //
    //    if (it == dataPool.end())
    //    {
    //        texture = Texture::CreateTexture(texturePath, textureName);
    //        dataPool.emplace(texture_id, texture);
    //    }
    //    else
    //    {
    //        texture = std::static_pointer_cast<Texture>(it->second);
    //    }
    //
    //    return texture;
    //}

    //Shr<Texture> DataPool::GetTexture(std::string textureName, bool wholePath) {
    //    // set default path
    //    const std::filesystem::path path = wholePath ? textureName : "resources/textures/" + textureName;
    //
    //    std::string texture_id = "recourcetexture_" + textureName;
    //
    //    Map::iterator it = dataPool.find(texture_id);
    //
    //    Shr<Texture> texture;
    //
    //    if (it == dataPool.end())
    //    {
    //        texture = Texture::CreateTexture(TextureSpecification(), path);
    //        dataPool.emplace(texture_id, texture);
    //    }
    //    else
    //    {
    //        texture = std::static_pointer_cast<Texture>(it->second);
    //    }
    //
    //    return texture;
    //}

    Ref<Font> DataPool::GetDefaultFont()
	{
        return GetFont("resources/fonts/mononoki.ttf", true, true);
    }

    Ref<Font> DataPool::GetFont(std::string fontName, bool wholePath, bool isDefault)
	{
        std::filesystem::path fontPath;
        if (wholePath)
            fontPath = Project::GetProjectPath() / fontName;
        else
            fontPath = Project::GetAssetTexturesPath() / fontName;

        if (isDefault)
            fontPath = fontName;

        std::string fontID = "font_" + fontName;

        const auto it = dataPool.find(fontID);

        std::shared_ptr<Font> font;

        if (it == dataPool.end())
        {
            
            font = MakeShr<Font>(fontPath, isDefault);
            dataPool.emplace(fontID, font);
        }
        else
        {
            font = std::static_pointer_cast<Font>(it->second);
        }

        return font;
    }

    void DataPool::ErasePool()
    {
        dataPool.clear();
    }

}
