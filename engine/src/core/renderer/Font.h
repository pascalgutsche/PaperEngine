#pragma once

#include "Engine.h"
#include "utility.h"

#include "renderer/Texture.h"

#include "renderer/MSDFData.h"

#include <filesystem>

namespace Paper
{
	class Font
	{
	public:
		Font(const std::filesystem::path& path, bool isDefault = false); // crappy workaround till asset rewrite
		~Font();

		MSDFData* GetMSDFData() const { return data; };
		Shr<Texture> GetAtlasTexture() const { return atlasTexture; };

		std::string GetFilePath() const { return fontPath.string(); }
		std::string GetFontName() const { return fontPath.filename().string(); }

		//see ctr
		bool IsDefault() const { return isDefault; }
	private:
		MSDFData* data;
		std::filesystem::path fontPath;
		Shr<Texture> atlasTexture;

		//see ctr
		bool isDefault;
	};

};