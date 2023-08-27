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

	void Project::SetActive(const Shr<Project>& project) const
	{
		activeProject = project;
	}
}

