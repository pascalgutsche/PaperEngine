#pragma once

#include "_Core.h"
#include "utility.h"

#include "renderer/Texture.h"

#include <filesystem>

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

		Shr<Font> GetDefault();

	private:
		MSDFData* data;
		Shr<Texture> atlasTexture;
	};

};