#pragma once
#include "Engine.h"

#include "asset/Asset.h"
#include "asset/AssetType.h"

namespace Paper
{
	class AssetManagerBase : public ShrCounted
	{
	public:
		AssetManagerBase() = default;
		virtual ~AssetManagerBase() = default;

		virtual bool IsAssetHandleValid(AssetHandle handle) = 0;
		virtual AssetType GetAssetType(AssetHandle handle) = 0;
		virtual const AssetMetadata& GetMetadata(AssetHandle handle) = 0;
		virtual const AssetMetadata& GetMetadata(const std::filesystem::path& path) = 0;
		virtual Shr<Asset> GetAsset(AssetHandle handle) = 0;

	};
}
