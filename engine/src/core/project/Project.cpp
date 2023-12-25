#include "Engine.h"
#include "Project.h"

#include "asset/manager/EditorAssetManager.h"

namespace Paper
{
	Project::Project(const ProjectConfig& config)
		: config(config)
	{
	}

	const ProjectConfig& Project::GetConfig() const
	{
		return config;
	}

	ProjectConfig& Project::GetConfig()
	{
		return config;
	}

	void Project::SetActive(const Ref<Project>& project)
	{
		activeProject = project;
		assetManager = Shr<EditorAssetManager>::Create();
	}

	Ref<Project> Project::GetActive()
	{
		return activeProject;
	}
}

