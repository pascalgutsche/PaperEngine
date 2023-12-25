#pragma once
#include "Engine.h"
#include "utility.h"
#include "renderer/Texture.h"

namespace Paper {

    using Map = std::unordered_map<std::string, std::shared_ptr<void>>;

    class Shader;
    class Texture;
    class Font;

    class DataPool {
    private:
        static Map dataPool;

    public:
        // get current shader with the specific string (name)
        static Ref<Shader> GetShader(std::string shaderName);
        // search for a texture with the specific string (name)
        static Shr<Texture> GetAssetTexture(std::string textureName, bool wholePath = false) { return nullptr; };
        static Shr<Texture> GetTexture(std::string textureName, bool wholePath = false) { return nullptr; };

        static Ref<Font> GetDefaultFont();
        static Ref<Font> GetFont(std::string fontName, bool wholePath = false, bool isDefault = false);

        static void ErasePool();
    };
}