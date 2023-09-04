#include "Editor.h"
#include "SettingsPanel.h"

#include "project/ProjectSerializer.h"

namespace PaperED
{
	void SettingsPanel::OnImGuiRender(bool& isOpen)
	{
		ImGui::Begin(panelName.c_str(), &isOpen);

		if (Project::GetActive() && ImGui::TreeNode("Project Settings"))
		{
			bool serialize = false;

			ProjectConfig& config = Project::GetActive()->GetConfig();

			ImGui::BeginDisabled();

			ImGui::InputText("Project Name:", &config.projectName);
			std::string projPath = config.projectPath.string();
			ImGui::InputText("Project Path:", &projPath);
			std::string projAssetPath = config.assetPath.string();
			ImGui::InputText("Project Asset path:", &projAssetPath);

			ImGui::EndDisabled();

			std::string scriptBinaryPath = config.scriptBinaryPath.string();
			if (ImGui::InputText("Script Binary Path:", &scriptBinaryPath, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				config.scriptBinaryPath = scriptBinaryPath;
				serialize = true;
			};

			std::string startSceneFilePath = config.startScene.string();
			if (ImGui::InputText("Start Scene:", &startSceneFilePath, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				config.startScene = startSceneFilePath;
				serialize = true;
			};

			if (serialize)
			{
				ProjectSerializer::Serialize(Project::GetActive(), Project::GetProjectPath() / "Project.pproj");
			}

			ImGui::TreePop();
		}

		ImGui::End();
	}
}

