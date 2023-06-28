#include "Editor.h"
#include "ProjectManager.h"

Shr<FileDialog> ProjectManager::fileDialog;

void ProjectManager::Init()
{
	fileDialog = FileDialog::Create();
}

std::filesystem::path ProjectManager::OpenFile(std::string filter)
{
	return fileDialog->OpenFile(filter);
}

std::filesystem::path ProjectManager::SaveFile(std::string filter)
{
	return fileDialog->OpenFile(filter);
}
