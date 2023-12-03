#include "Engine.h"
#include "Project.h"

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
	}

	Ref<Project> Project::GetActive()
	{
		return activeProject;
	}
}

