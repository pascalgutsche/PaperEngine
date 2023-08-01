#include "Editor.h"
#include "PaperLayer.h"

#include "WindowsOpen.h"

void PaperLayer::CameraSettingsPanel()
{
	ImGui::Begin("Camera Settings", &show_camera_settings_panel, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::BeginTabBar("##tabs"))
	{
		for (auto& viewport : viewports)
		{
			if (ImGui::BeginTabItem(viewport.name.c_str()))
			{
				const Shr<EditorCamera> camera = viewport.camera;

				ImGui::DragFloat3("Position", &camera->position.x, 0.1f);
				ImGui::DragFloat("Yaw", &camera->yaw, 0.1f);
				ImGui::DragFloat("Pitch", &camera->pitch, 0.1f);
				ImGui::DragFloat("Roll", &camera->roll, 0.1f);

				ImGui::DragFloat("FOV", &camera->fov, 0.1f);

				ImGui::DragFloat("Far-Plane", &camera->far_plane, 0.01f);
				ImGui::DragFloat("Near-Plane", &camera->near_plane, 0.01f);
				ImGui::DragFloat("Left_Plane", &camera->left_frostum, 0.01f);
				ImGui::DragFloat("Right-Plane", &camera->right_frostum, 0.01f);
				ImGui::DragFloat("Top-Plane", &camera->top_frostum, 0.01f);
				ImGui::DragFloat("Bottom-Plane", &camera->bottom_frostum, 0.01f);

				ImGui::DragFloat3("Target", &camera->target.x, 0.1f);


				if (ImGui::Button("Reset Camera Values"))
					camera->ResetValues();

				ImGui::EndTabItem();

			}
		}


		ImGui::EndTabBar();
	};


	ImGui::End();


	camera_settings_panel_first = false;
}
