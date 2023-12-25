#pragma once
#include "Engine.h"
#include "utility.h"
#include "generic/Buffer.h"

#include "renderer/Texture.h"

#include "glad/glad.h"

namespace Paper
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(TextureSpecification specification, std::filesystem::path filePath);
		OpenGLTexture(TextureSpecification specification);
		~OpenGLTexture() override;

		void Bind(unsigned slot) override;
		void Unbind() override;

		void SetData(void* data, uint32_t size) override;

		bool IsLoaded() override { return textureData; }

		uint32_t GetID() const override { return texID; }
		const std::filesystem::path& GetFilePath() const override { return filePath; }
		const TextureSpecification& GetSpecification() const override { return specification; }

		bool operator==(const Texture& other) const override { return texID == other.GetID(); }
	private:
		TextureSpecification specification;

		GLenum internalFormat;
		GLenum dataFormat;

		std::filesystem::path filePath;

		Buffer textureData;

		uint32_t texID = 0;

		static Buffer LoadDataToBuffer(const std::filesystem::path& filePath, int& outWidth, int& outHeight, int& outChannels);
	};
}