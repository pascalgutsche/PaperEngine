#include "Engine.h"
#include "AssetSerializer.h"

#include "scene/SceneSerializer.h"


namespace Paper
{
	void SceneAssetSerializer::Serialize(const AssetMetadata& metadata, const Ref<Asset>& asset)
	{
			
		//SceneSerializer::Serialize()
	}

	bool SceneAssetSerializer::Deserialize(const AssetMetadata& metadata, Ref<Asset>& asset)
	{
		return false;
	}

}
