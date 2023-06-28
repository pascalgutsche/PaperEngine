#pragma once
#include "Editor.h"

class ProjectManager
{
public:
	static void Init();

	static std::filesystem::path OpenFile(std::string filter);
	static std::filesystem::path SaveFile(std::string filter);

private:
	static Shr<FileDialog> fileDialog;
};

