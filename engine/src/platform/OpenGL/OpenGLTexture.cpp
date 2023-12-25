#include "Engine.h"
#include "OpenGLTexture.h"

#include "renderer/Texture.h"

#include <glad/glad.h>
#include <STB_IMAGE/stb_image.h>


namespace Paper
{

	static GLenum ImageFormatToGLDataFormat(ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::RGB8:  return GL_RGB;
		case ImageFormat::RGBA8: return GL_RGBA;
		}

		CORE_ASSERT("",  false);
		return 0;
	}

	static GLenum ImageFormatToGLInternalFormat(ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::RGB8:  return GL_RGB8;
		case ImageFormat::RGBA8: return GL_RGBA8;
		}

		CORE_ASSERT("", false);
		return 0;
	}
	OpenGLTexture::OpenGLTexture(TextureSpecification specification, std::filesystem::path filePath)
		: specification(specification), filePath(filePath)
	{
		glGenTextures(1, &texID);
		// use texture (everything that is called from now will be set to the current texture)
		glBindTexture(GL_TEXTURE_2D, texID);
		// set texture parameters

		// repeat image in both directions (activate the repeating of the texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// pixelate the texture when made bigger
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// pixelate the texture when made smaller
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		// load texture and save formats to the variables (4 == RGBA format)
		int channels = 0;
		textureData = LoadDataToBuffer(filePath, specification.width, specification.height, channels);

		if (!textureData)
		{
			textureData = LoadDataToBuffer("resources/textures/error_texture_256x256.png", specification.width, specification.height, channels);
			LOG_CORE_ERROR("Could not load image '" + filePath.string() + "'");
		}

		if (channels == 3) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, specification.width, specification.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.data);
			specification.format = ImageFormat::RGB8;
		}
		else if (channels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, specification.width, specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
			specification.format = ImageFormat::RGBA8;
		}
		else {
			LOG_CORE_ERROR("Unknown number of channel '" + std::to_string(channels) + "' by texture '" + filePath.string() + "'");
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		internalFormat = ImageFormatToGLInternalFormat(specification.format);
		dataFormat = ImageFormatToGLDataFormat(specification.format);
	}

	OpenGLTexture::OpenGLTexture(TextureSpecification specification)
		: specification(specification)
	{

		internalFormat = ImageFormatToGLInternalFormat(specification.format);
		dataFormat = ImageFormatToGLDataFormat(specification.format);

		glCreateTextures(GL_TEXTURE_2D, 1, &texID);
		glTextureStorage2D(texID, 1, internalFormat, specification.width, specification.height);

		glTextureParameteri(texID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(texID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(texID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(texID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void OpenGLTexture::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = dataFormat == GL_RGBA ? 4 : 3;
		CORE_ASSERT(size == specification.width * specification.height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(texID, 0, 0, 0, specification.width, specification.height, dataFormat, GL_UNSIGNED_BYTE, data);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &texID);
	}

	void OpenGLTexture::Bind(unsigned slot)
	{
		if (slot < 31)
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, texID);
			return;
		}
		LOG_CORE_WARN("You should not go over 31 texture slots, as the OpenGL specification does not allow more");
	}

	void OpenGLTexture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Buffer OpenGLTexture::LoadDataToBuffer(const std::filesystem::path& filePath, int& outWidth, int& outHeight,
		int& outChannels)
	{
		Buffer buffer;

		stbi_set_flip_vertically_on_load(true);
		buffer.data = stbi_load(filePath.string().c_str(), &outWidth, &outHeight, &outChannels, 0);
		buffer.size = outWidth * outHeight * outChannels;

		if (!buffer)
			return {};
		return buffer;
	}
}
