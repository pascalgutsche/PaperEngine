#pragma once
#include "Engine.h"
#include "Asset.h"

namespace Paper
{
	class AssetSerializer
	{
	public:
		virtual ~AssetSerializer() = default;

		virtual void Serialize(const AssetMetadata& metadata, const Ref<Asset>& asset) = 0;

		virtual bool Deserialize(const AssetMetadata& metadata, Ref<Asset>& asset) = 0;
	};

	class SceneAssetSerializer : public AssetSerializer
	{
	public:
		~SceneAssetSerializer() override;

		void Serialize(const AssetMetadata& metadata, const Ref<Asset>& asset) override;
		bool Deserialize(const AssetMetadata& metadata, Ref<Asset>& asset) override;
	};
}


