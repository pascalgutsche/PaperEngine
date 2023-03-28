#pragma once

#include "_Core.h"
#include "utility.h"

#include <filesystem>

#include "renderer/Texture.h"

namespace core
{
	struct MSDFData;

	class Font 
	{
	public:
		Font(const std::filesystem::path& font);
		~Font();

		const MSDFData* GetMSDFData() const { return data; };
		Shr<Texture> GetAtlasTexture() const { return atlasTexture; };

		Shr<Font> Font::GetDefault();

	private:
		MSDFData* data;
		Shr<Texture> atlasTexture;

	};

}
