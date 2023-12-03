#pragma once
#include "Engine.h"

namespace Paper
{
	enum class AssetType
	{
		None,
		Scene,
		Texture,
		Font,
		Script
	};

	inline AssetType GetAssetTypeFromExtension(const std::string& extension)
	{
		// Paper scene
		if (extension == ".pscene") return AssetType::Scene;

		//Textures
		if (extension == ".png") return AssetType::Texture;
		if (extension == ".jpg") return AssetType::Texture;
		if (extension == ".jpeg") return AssetType::Texture;

		//Fonts
		if (extension == ".ttf") return AssetType::Font;
		if (extension == ".otf") return AssetType::Font;

		//Scripts
		if (extension == ".cs") return AssetType::Script;

		return AssetType::None;
	}

	namespace Utils
	{
		inline std::string ConvertAssetTypeToString(AssetType type)
		{
			switch(type)
			{
			case AssetType::None: return "None";
			case AssetType::Scene: return "Scene";
			case AssetType::Texture: return "Texture";
			case AssetType::Font: return "Font";
			case AssetType::Script: return "Script";
			}
			CORE_ASSERT(false, "");
			return "";
		}

		inline AssetType ConvertStringToAssetType(std::string type)
		{
			if (type == "None") return AssetType::None;
			if (type == "Scene") return AssetType::Scene;
			if (type == "Texture") return AssetType::Texture;
			if (type == "Font") return AssetType::Font;
			if (type == "Script") return AssetType::Script;
			
			CORE_ASSERT(false, "");
			return AssetType::None;
		}
	}
}