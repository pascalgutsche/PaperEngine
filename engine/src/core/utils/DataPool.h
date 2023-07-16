#pragma once
#include "Engine.h"
#include "utility.h"

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
        static Shr<Shader> GetShader(std::string shaderName);
        // search for a texture with the specific string (name)
        static Shr<Texture> GetAssetTexture(std::string textureName, bool wholePath = false);
        static Shr<Texture> GetTexture(std::string textureName, bool wholePath = false);

        static Shr<Font> GetFont(std::string fontName);

        static void ErasePool();
    };

    enum class ProjectionMode { PERSPECTIVE, ORTHOGRAPHIC, SCREEN };
    int ProjectionModeToInt(const ProjectionMode& mode);
}