#pragma once

#include "_Core.h"
#include "utility.h"

#include "renderer/Texture.h"

#include "renderer/MSDFData.h"

#include <filesystem>

namespace core
{
	class Font
	{
	public:
		Font(const std::filesystem::path& font);
		~Font();

		MSDFData* GetMSDFData() const { return data; };
		Shr<Texture> GetAtlasTexture() const { return atlasTexture; };

		static Shr<Font> GetDefault();

	private:
		MSDFData* data;
		Shr<Texture> atlasTexture;
	};

};