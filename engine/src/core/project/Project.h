#pragma once
#include "Engine.h"

#include "scene/SceneSerializer.h"

namespace Paper
{
	class Scene;

	struct ProjectConfig
	{
		std::string projectName = "New Project";
		std::filesystem::path projectPath;
		std::filesystem::path assetPath = "assets";
		std::filesystem::path scriptBinaryPath = "assets/scripts/bin";
		std::filesystem::path startScene;
	};

	class Project
	{
	public:
		Project() = default;
		Project(const ProjectConfig& config);
		~Project() = default;

		const ProjectConfig& GetConfig() const;
		ProjectConfig& GetConfig();

		static void SetActive(const Shr<Project>& project);
		static Shr<Project> GetActive();

		static std::string GetProjectName()
		{
			CORE_ASSERT(activeProject, "");
			return activeProject->GetConfig().projectName;
		}

		static std::filesystem::path GetProjectPath()
		{
			CORE_ASSERT(activeProject, "");
			return activeProject->GetConfig().projectPath;
		}

		static std::filesystem::path GetAssetPath()
		{
			CORE_ASSERT(activeProject, "");
			return activeProject->GetConfig().projectPath / activeProject->GetConfig().assetPath;
		}

		static std::filesystem::path GetAssetScenesPath()
		{
			CORE_ASSERT(activeProject, "");
			return GetAssetPath() / "scenes";
		}

		static std::filesystem::path GetAssetTexturesPath()
		{
			CORE_ASSERT(activeProject, "");
			return GetAssetPath() / "textures";
		}

		static std::filesystem::path GetScriptBinaryPath()
		{
			CORE_ASSERT(activeProject, "");
			return activeProject->GetConfig().projectPath / activeProject->GetConfig().scriptBinaryPath;
		}

		static std::filesystem::path GetScriptBinaryFilePath()
		{
			CORE_ASSERT(activeProject, "");
			return GetScriptBinaryPath() / fmt::format("{}.dll", GetProjectName());
		}

		static Shr<Scene> GetStartScene()
		{
			CORE_ASSERT(activeProject, "");
			if (!activeProject->GetConfig().startScene.empty())
				return SceneSerializer::Deserialize(GetProjectPath() / activeProject->GetConfig().startScene);
			return nullptr;
		}

		static std::filesystem::path GetRelativePathFromProject(const std::filesystem::path& filePath)
		{
			std::string path = filePath.string();
			const size_t pos = path.find(GetProjectPath().string());
			if (pos != std::string::npos)
			{
				path.erase(path.begin(), path.begin() + pos + GetProjectPath().string().length() + 1); // +1 because the slash of the path
			}
			return path;
		}

	private:
		ProjectConfig config;

		static inline Shr<Project> activeProject = nullptr;

		friend class ProjectSerializer;
	};
}

