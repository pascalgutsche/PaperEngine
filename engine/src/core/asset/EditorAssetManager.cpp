#include "Engine.h"

#include "EditorAssetManager.h"

#include "project/Project.h"

namespace Paper
{
	EditorAssetManager::EditorAssetManager()
		:assetRegistry(), assetsLoaded()
	{

	}

	AssetType EditorAssetManager::GetAssetType(AssetHandle handle)
	{
		const auto metadata = GetMetadata(handle);
		if (!metadata.IsValid())
			return AssetType::None;

		return metadata.type;
	}

	Ref<Asset> EditorAssetManager::GetAsset(AssetHandle handle)
	{
		const auto metadata = GetMetadata(handle);
		if (!metadata.IsValid())
			return nullptr;

		//TODO: LOAD ASSETS
		// ...
		return nullptr;
	}

	const AssetMetadata& EditorAssetManager::GetMetadata(AssetHandle handle)
	{
		if (assetRegistry.contains(handle))
			return assetRegistry[handle];

		return AssetMetadata();
	}

	const AssetMetadata& EditorAssetManager::GetMetadata(const std::filesystem::path& path)
	{
		std::filesystem::path relativePath = std::filesystem::relative(path.lexically_normal(), Project::GetAssetPath());

		for (const auto metadata : assetRegistry | std::views::values)
		{
			if (metadata.filePath == relativePath)
				return metadata;
		}

		return AssetMetadata();
	}

	AssetHandle EditorAssetManager::AddAsset(const std::filesystem::path& path)
	{
		return AssetHandle();
	}

	bool EditorAssetManager::RemoveAsset(const std::filesystem::path& path)
	{
		return false;
	}
}
