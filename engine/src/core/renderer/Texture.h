#pragma once
#include "Engine.h"
#include "utility.h"
#include "asset/Asset.h"

namespace Paper {

    enum class ImageFormat
    {
        None = 0,
        R8,
        RGB8,
        RGBA8,
        RGBA32F
    };

    struct TextureSpecification
    {
        int32_t width = 1;
        int32_t height = 1;
        ImageFormat format = ImageFormat::None;
        bool generateMips = true;

        float GetRatio() const
        {
            return width / height;
        }
    };

    class Texture : public Asset {
    private:
    public:
        ~Texture() override = default;

    	virtual void Bind(unsigned int slot) = 0;
        virtual void Unbind() = 0;

        virtual void SetData(void* data, uint32_t size) = 0;

        virtual bool IsLoaded() = 0;

        virtual uint32_t GetID() const = 0;
    	virtual const std::filesystem::path& GetFilePath() const = 0;
        virtual const TextureSpecification& GetSpecification() const = 0;

        virtual bool operator==(const Texture& other) const = 0;

        static Shr<Texture> CreateTexture(TextureSpecification specification, std::filesystem::path filePath);
        static Shr<Texture> CreateTexture(TextureSpecification specification);
    };

}
