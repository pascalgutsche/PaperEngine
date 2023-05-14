#pragma once
#include "Editor.h"

class ProjectManager
{
public:
	static void Init();

	static std::string OpenFile(std::string filter);
	static std::string SaveFile(std::string filter);

private:
	static Shr<FileDialog> fileDialog;
};

