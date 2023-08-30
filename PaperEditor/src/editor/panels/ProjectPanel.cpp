#include "Editor.h"
#include "ProjectPanel.h"

#include "editor/PaperLayer.h"

#include "project/ProjectManager.h"
#include "utils/FileSystem.h"

#include <imgui/misc/cpp/imgui_stdlib.h>
#include <nfd.h>


namespace PaperED
{
	void NewProjectPanel::OnImGuiRender(bool& isOpen)
	{
		ImGui::Begin(panelName.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);

		std::string fullProjPath = "";
		if (!projPath.empty())
		{
			fullProjPath = fmt::format("{}\\{}", projPath, projName);
		}

		ImGui::Text(fmt::format("Project is created in: {}", fullProjPath).c_str());

		ImGui::InputText("Name: ", &projName);
		ImGui::InputText("Path: ", &projPath);
		ImGui::SameLine();
		if (ImGui::Button("..."))
			projPath = FileSystem::OpenFolder().string();

		if (ImGui::Button("Cancel"))
			isOpen = false;

		ImGui::SameLine();

		if (ImGui::Button("Create"))
			paperLayer->CreateProject(projPath, projName);

		ImGui::End();
	}
}
