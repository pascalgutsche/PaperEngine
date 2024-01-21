#pragma once
#include "Engine.h"

#include "asset/manager/AssetManagerBase.h"

namespace Paper
{
	class EditorAssetManager : public AssetManagerBase
	{
	public:
		EditorAssetManager();

		~EditorAssetManager() override = default;

		bool IsAssetHandleValid(AssetHandle handle) override;

		AssetType GetAssetType(AssetHandle handle) override;
		Shr<Asset> GetAsset(AssetHandle handle) override;

		const AssetMetadata& GetMetadata(AssetHandle handle) override;
		const AssetMetadata& GetMetadata(const std::filesystem::path& path) override;

		AssetHandle AddAsset(const std::filesystem::path& path);
		bool RemoveAsset(const std::filesystem::path& path);
		bool RemoveAsset(AssetHandle handle);

		void ReloadAssets();

		void UnloadAllAssets();

	private:
		bool SerializeRegistry();
		bool DeserializeRegistry();

		void SearchForAssets(const std::filesystem::path& path);

		static AssetMetadata invalidMetadata;

		AssetMetadata& GetMutableMetadata(AssetHandle handle);

		std::unordered_map<AssetHandle, AssetMetadata> assetRegistry;
		std::unordered_map<AssetHandle, Shr<Asset>> assetsLoaded;
	};
}