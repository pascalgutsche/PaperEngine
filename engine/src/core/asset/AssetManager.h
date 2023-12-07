#pragma once
#include "Engine.h"

#include "Asset.h"
#include "AssetType.h"

namespace Paper
{
	class AssetManager : public ShrCounted
	{
	public:
		AssetManager() = default;
		virtual ~AssetManager() = default;

		virtual AssetType GetAssetType(AssetHandle handle) = 0;
		virtual const AssetMetadata& GetMetadata(AssetHandle handle) = 0;
		virtual const AssetMetadata& GetMetadata(const std::filesystem::path& path) = 0;
		virtual Ref<Asset> GetAsset(AssetHandle handle) = 0;

	};
}
