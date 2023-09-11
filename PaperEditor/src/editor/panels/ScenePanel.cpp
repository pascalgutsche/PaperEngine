#include "Editor.h"
#include "ScenePanel.h"

namespace PaperED
{
	void ScenePanel::OnImGuiRender(bool& isOpen)
	{
		Shr<Scene> activeScene = Scene::GetActive();

		ImGui::Begin(panelName.c_str(), &isOpen);

		if (!activeScene)
		{
			ImGui::Text("No active scene");
			ImGui::End();
			return;
		}

		ImGui::Text("Name: {}", activeScene->GetName());
		ImGui::Text("Path: {}", Project::GetRelativePathFromProject(activeScene->GetPath()));
		ImGui::Text("Number of Entities: {}", activeScene->EntityMap().size());

		if (ImGui::TreeNode("Settings"))
		{
			float textX = ImGui::CalcTextSize("Physics").x;
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x - textX) / 2);
			ImGui::Text("Physics");
			ImGui::Separator();


			ImGui::TreePop();
		};

		


		ImGui::End();
	}
}

