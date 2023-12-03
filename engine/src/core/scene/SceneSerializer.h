#pragma once
#include "Engine.h"

#include "Scene.h"

namespace Paper
{
	class SceneSerializer
	{
	public:
		static void Serialize(const Ref<Scene>& scene, const std::filesystem::path& filePath);
		static void Serialize(const Ref<Scene>& scene, std::string& outString);
		static void Serialize(const Ref<Scene>& scene, std::string& outString, const std::filesystem::path& newPath);
		static Ref<Scene> Deserialize(const std::filesystem::path& filePath);

		static bool IsSceneDirty(const Ref<Scene>& scene);
	};
}
