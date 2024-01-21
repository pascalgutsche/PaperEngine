#include "Engine.h"

#include "EditorAssetManager.h"

#include "asset/AssetImporter.h"
#include "project/Project.h"
#include "utils/FileSystem.h"

#include "serializer/YAMLSerializer.h"

namespace Paper
{
	AssetMetadata EditorAssetManager::invalidMetadata;

	EditorAssetManager::EditorAssetManager()
		:assetRegistry(), assetsLoaded()
	{
		AssetImporter::Init();

		DeserializeRegistry();
		ReloadAssets();
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
		for (const auto metadata : assetRegistry | std::views::values)
		{
			if (metadata.filePath == path)
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

	void EditorAssetManager::ReloadAssets()
	{
		SearchForAssets(Project::GetAssetPath());
		UnloadAllAssets();
		SerializeRegistry();
	}

	void EditorAssetManager::UnloadAllAssets()
	{
		for (auto& [handle, metadata] : assetRegistry)
		{
			metadata.isDataLoaded = false;
			metadata.errorWhileImporting = false;
		}
		assetsLoaded.clear();
	}

	bool EditorAssetManager::SerializeRegistry()
	{
		std::filesystem::path registryPath = Project::GetAssetRegistryFilePath();

		std::map<const AssetHandle, AssetMetadata> tmpRegistry;

		for (auto [handle, metadata] : assetRegistry)
		{
			tmpRegistry[handle] = AssetMetadata(metadata);
		}

		LOG_TRACE("Serializing AssetRegistry to '{}' with {} entries!", registryPath, tmpRegistry.size());

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetRegistry" << YAML::BeginSeq;


		for (const auto& [handle, metadata] : tmpRegistry)
		{
			out << YAML::BeginMap;
			out << YAML::Key << "AssetHandle" << YAML::Value << handle;
			out << YAML::Key << "FilePath" << YAML::Value << metadata.filePath;
			out << YAML::Key << "Type" << YAML::Value << Utils::ConvertAssetTypeToString(metadata.type);
			out << YAML::EndMap;
		}


		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(registryPath.string());
		fout << out.c_str();
		fout.close();

		return true;
	}

	bool EditorAssetManager::DeserializeRegistry()
	{
		std::filesystem::path registryPath = Project::GetAssetRegistryFilePath();
		if (!FileSystem::Exists(registryPath))
		{
			LOG_CORE_ERROR("Could not find asset registry: '{}'", registryPath.string());
			CORE_ASSERT(false, "")
			return false;
		}

		LOG_TRACE("Loading AssetRegistry from '{}'", registryPath);

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(registryPath.string());
		}
		catch (YAML::Exception e)
		{
			LOG_CORE_ERROR("Failed to asset registry file '{0}'\n\t{1}", registryPath, e.what());
			CORE_ASSERT(false, "")
			return false;
		}

		try
		{
			auto registry = data["AssetRegistry"];
			for (auto entry : registry)
			{
				std::string filePath = entry["FilePath"].as<std::string>();

				AssetMetadata metadata;
				metadata.handle = entry["AssetHandle"].as<AssetHandle>();
				metadata.filePath = filePath;
				metadata.type = Utils::ConvertStringToAssetType(entry["Type"].as<std::string>());

				if (!metadata.IsValid())
					continue;

				assetRegistry[metadata.handle] = metadata;
			}
			LOG_TRACE("Loaded {} from AssetRegistry", assetRegistry.size());
		}
		catch (YAML::Exception& ex)
		{
			LOG_CORE_CRITICAL(ex.msg);
			CORE_ASSERT(false, "")
			return false;
		}


		return true;
	}

	void EditorAssetManager::SearchForAssets(const std::filesystem::path& path)
	{
		for (auto entry : std::filesystem::recursive_directory_iterator(path))
		{
			if (entry.is_regular_file())
			{
				AddAsset(entry.path());
			}
		}
	}
}
