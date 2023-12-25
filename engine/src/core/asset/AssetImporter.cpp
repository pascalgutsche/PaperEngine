#include "Engine.h"
#include "AssetImporter.h"

namespace Paper
{
	std::unordered_map<AssetType, Scope<AssetSerializer>> AssetImporter::serializers;

	void AssetImporter::Init()
	{
		serializers.clear();
		serializers[AssetType::Texture] = MakeScoped<TextureSerializer>();
	}

	bool AssetImporter::TryLoadData(const AssetMetadata& metadata, Shr<Asset>& asset)
	{
		if (!serializers.contains(metadata.type))
		{
			LOG_CORE_ERROR("No importer for asset type: '{}'. Trying to import '{}'", Utils::ConvertAssetTypeToString(metadata.type), metadata.filePath.string());
			return false;
		}

		return serializers[metadata.type]->Deserialize(metadata, asset);
	}
}
