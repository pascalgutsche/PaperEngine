#pragma once
#include "_Core.h"
#include "utility.h"

#include "renderer/Texture2D.h"

#include "glad/glad.h"

namespace core
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture(std::string filePath, std::string name);
		OpenGLTexture(TextureSpecification specification);
		~OpenGLTexture() override;

		void Bind(unsigned slot) override;
		void Unbind() override;

		void SetData(void* data, uint32_t size) override;

		bool IsLoaded() override;

		uint32_t GetID() const override;
		int GetWidth() override;
		int GetHeight() override;

		void SetData(void* data, uint32_t size) override;

		bool IsLoaded() const override { return isLoaded; };

		bool operator==(const Texture& other) const override;

		std::string GetFilePath() override;

		std::string GetName() override;

	private:
		TextureSpecification specification;

		GLenum internalFormat;
		GLenum dataFormat;

		std::string filePath;
		std::string name;

		unsigned char* localBuffer;

		int width;
		int height;
		int channels;

		bool isLoaded = false;

		uint32_t texID;

		GLenum dataFormat;
		GLenum internalFormat;

		bool Init(std::string path);
	};
}