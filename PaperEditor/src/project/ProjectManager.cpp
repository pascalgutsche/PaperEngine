#include "Editor.h"
#include "ProjectManager.h"

Shr<FileDialog> ProjectManager::fileDialog;

void ProjectManager::Init()
{
	fileDialog = FileDialog::Create();
}

std::string ProjectManager::OpenFile(std::string filter)
{
	return fileDialog->OpenFile(filter);
}

std::string ProjectManager::SaveFile(std::string filter)
{
	return fileDialog->OpenFile(filter);
}
