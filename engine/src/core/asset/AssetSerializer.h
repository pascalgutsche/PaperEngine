#pragma once
#include "Engine.h"
#include "Asset.h"

namespace Paper
{
	class AssetSerializer
	{
	public:
		virtual ~AssetSerializer() = default;

		virtual void Serialize(const AssetMetadata& metadata, const Shr<Asset>& asset) = 0;

		virtual bool Deserialize(const AssetMetadata& metadata, Shr<Asset>& asset) = 0;
	};

	class SceneAssetSerializer : public AssetSerializer
	{
	public:
		~SceneAssetSerializer() override = default;

		void Serialize(const AssetMetadata& metadata, const Shr<Asset>& asset) override;
		bool Deserialize(const AssetMetadata& metadata, Shr<Asset>& asset) override;
	};

	class TextureSerializer : public AssetSerializer
	{
	public:
		~TextureSerializer() override = default;

		void Serialize(const AssetMetadata& metadata, const Shr<Asset>& asset) override;
		bool Deserialize(const AssetMetadata& metadata, Shr<Asset>& asset) override;
	};
	
}


