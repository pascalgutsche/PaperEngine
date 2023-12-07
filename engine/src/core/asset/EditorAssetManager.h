#pragma once
#include "Engine.h"

#include "AssetManager.h"

namespace Paper
{
	class EditorAssetManager : public AssetManager
	{
	public:
		EditorAssetManager();

		~EditorAssetManager() override = default;

		AssetType GetAssetType(AssetHandle handle) override;
		Ref<Asset> GetAsset(AssetHandle handle) override;

		const AssetMetadata& GetMetadata(AssetHandle handle) override;
		const AssetMetadata& GetMetadata(const std::filesystem::path& path) override;

		AssetHandle AddAsset(const std::filesystem::path& path);
		bool RemoveAsset(const std::filesystem::path& path);

	private:
		std::unordered_map<AssetHandle, AssetMetadata> assetRegistry;
		std::unordered_map<AssetHandle, Ref<Asset>> assetsLoaded;
	};
}