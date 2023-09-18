#pragma once
#include "Engine.h"

#include "Scene.h"

namespace Paper
{
	class SceneSerializer
	{
	public:
		static void Serialize(const Shr<Scene>& scene, const std::filesystem::path& filePath);
		static void Serialize(const Shr<Scene>& scene, std::string& outString);
		static void Serialize(const Shr<Scene>& scene, std::string& outString, const std::filesystem::path& newPath);
		static Shr<Scene> Deserialize(const std::filesystem::path& filePath);

		static bool IsSceneDirty(const Shr<Scene>& scene);
	};
}
