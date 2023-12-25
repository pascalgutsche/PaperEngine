#include "Engine.h"

#include "EditorAssetManager.h"

#include "asset/AssetImporter.h"
#include "project/Project.h"
#include "utils/FileSystem.h"

namespace Paper
{
	AssetMetadata EditorAssetManager::invalidMetadata;

	EditorAssetManager::EditorAssetManager()
		:assetRegistry(), assetsLoaded()
	{
		AssetImporter::Init();
	}

	bool EditorAssetManager::IsAssetHandleValid(AssetHandle handle)
	{
		const auto metadata = GetMetadata(handle);
		return metadata.IsValid();
	}

	AssetType EditorAssetManager::GetAssetType(AssetHandle handle)
	{
		const auto metadata = GetMetadata(handle);
		if (!metadata.IsValid())
			return AssetType::None;

		return metadata.type;
	}

	Shr<Asset> EditorAssetManager::GetAsset(AssetHandle handle)
	{
		auto& metadata = GetMutableMetadata(handle);
		if (!metadata.IsValid())
			return nullptr;

		if (metadata.errorWhileImporting) // already tried importing and failed
			return nullptr;

		Shr<Asset> asset = nullptr;
		if (!metadata.isDataLoaded)
		{
			metadata.isDataLoaded = AssetImporter::TryLoadData(metadata, asset);
			if (!metadata.isDataLoaded)
			{
				metadata.errorWhileImporting = true;
				return nullptr;
			}

			assetsLoaded[handle] = asset;
		}
		else
			asset = assetsLoaded[handle];
		return asset;
	}

	const AssetMetadata& EditorAssetManager::GetMetadata(AssetHandle handle)
	{
		if (assetRegistry.contains(handle))
			return assetRegistry[handle];

		return AssetMetadata();
	}

	AssetMetadata& EditorAssetManager::GetMutableMetadata(AssetHandle handle)
	{
		if (assetRegistry.contains(handle))
			return assetRegistry[handle];

		return invalidMetadata;
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
		const AssetMetadata& metadata = GetMetadata(path);
		if (metadata.IsValid())
			return metadata.handle;

		if (FileSystem::Exists(path))
		{
			AssetType type = GetAssetTypeFromExtension(path.extension().string());
			if (type == AssetType::None)
				return 0;
			AssetMetadata newMetadata;
			newMetadata.filePath = path;
			newMetadata.handle = AssetHandle();
			newMetadata.type = type;

			assetRegistry[newMetadata.handle] = newMetadata;
			return newMetadata.handle;
		}
		return 0;
	}

	bool EditorAssetManager::RemoveAsset(const std::filesystem::path& path)
	{
		for (const auto [assetHandle, assetMetadata] : assetRegistry)
		{
			if (assetMetadata.filePath == path)
			{
				assetRegistry.erase(assetHandle);
				return true;
			}
		}
		return false;
	}

	bool EditorAssetManager::RemoveAsset(AssetHandle handle)
	{
		if (!assetRegistry.contains(handle))
			return false;

		assetRegistry.erase(handle);
		return true;
	}
}
