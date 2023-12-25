#pragma once
#include "asset/Asset.h"
#include "project/Project.h"


namespace Paper
{
	class AssetManager
	{
	public:

		static bool IsAssetHandleValid(AssetHandle handle);
		static AssetType GetAssetType(AssetHandle handle);

		template <typename TType>
		static Shr<TType> GetAsset(AssetHandle handle)
		{
			return Project::GetAssetManagerBase()->GetAsset(handle);
		}

	};
}
