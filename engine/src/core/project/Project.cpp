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

	void Project::SetActive(const Shr<Project>& project)
	{
		activeProject = project;
	}

	Shr<Project> Project::GetActive()
	{
		return activeProject;
	}
}

