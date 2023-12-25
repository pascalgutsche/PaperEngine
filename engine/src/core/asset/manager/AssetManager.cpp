#include "Engine.h"
#include "AssetManager.h"

namespace Paper
{
	bool AssetManager::IsAssetHandleValid(AssetHandle handle)
	{
		return Project::GetAssetManagerBase()->IsAssetHandleValid(handle);
	}

	AssetType AssetManager::GetAssetType(AssetHandle handle)
	{
		return Project::GetAssetManagerBase()->GetAssetType(handle);
	}
}

