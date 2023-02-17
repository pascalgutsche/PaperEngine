#pragma once
#include "_Core.h"
#include "utility.h"

#include "renderer/Texture.h"

namespace core
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(std::string filePath, std::string name);
		~OpenGLTexture() override;

		void Bind(unsigned slot) override;
		void Unbind() override;

		int GetID() override;
		int GetWidth() override;
		int GetHeight() override;

		std::string GetFilePath() override;
		std::string GetName() override;

	private:
		std::string filePath;
		std::string name;

		unsigned char* localBuffer;

		int width;
		int height;
		int channels;

		unsigned int texID;

		bool Init(std::string path);
	};
}