#include "Engine.h"
#include "AssetSerializer.h"

#include "renderer/Texture.h"
#include "scene/SceneSerializer.h"


namespace Paper
{
	void SceneAssetSerializer::Serialize(const AssetMetadata& metadata, const Shr<Asset>& asset)
	{
		
	}

	bool SceneAssetSerializer::Deserialize(const AssetMetadata& metadata, Shr<Asset>& asset)
	{
		return false;
	}

	void TextureSerializer::Serialize(const AssetMetadata& metadata, const Shr<Asset>& asset)
	{

	}

	bool TextureSerializer::Deserialize(const AssetMetadata& metadata, Shr<Asset>& asset)
	{
		asset = Texture::CreateTexture(TextureSpecification(), metadata.filePath);
		asset->handle = metadata.handle;

		bool valid = asset.As<Texture>()->IsLoaded();

		if (!valid)
			asset->SetFlag(AssetFlag::Invalid);

		return valid;
	}
}
