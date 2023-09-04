#pragma once
#include "Engine.h"

#include "Project.h"

namespace Paper
{
	class ProjectSerializer
	{
	public:
		static void Serialize(const Shr<Project>& project, const std::filesystem::path& filePath);
		static Shr<Project> Deserialize(const std::filesystem::path& filePath);
	};
}

