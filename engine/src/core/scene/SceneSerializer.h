#pragma once
#include "Engine.h"

#include "Scene.h"

namespace Paper
{
	class SceneSerializer
	{
	public:
		static void Serialize(const Shr<Scene>& scene, const std::filesystem::path& filePath);
		static Shr<Scene> Deserialize(const std::filesystem::path& filePath);
	};
}
