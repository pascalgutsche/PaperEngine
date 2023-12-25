#pragma once
#include "Asset.h"
#include "AssetSerializer.h"

namespace Paper
{
	class AssetImporter
	{
	public:
		static void Init();

		static bool TryLoadData(const AssetMetadata& metadata, Shr<Asset>& asset);


	private:
		static std::unordered_map<AssetType, Scope<AssetSerializer>> serializers;
	};
}
