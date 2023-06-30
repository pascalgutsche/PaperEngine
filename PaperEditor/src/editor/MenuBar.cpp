#include "Editor.h"

#include "WindowsOpen.h"

#include "PELayer.h"
#include "project/ProjectManager.h"

void MenuTabView()
{
	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("Asset Manager"))
			show_asset_manager_panel = true;

		if (ImGui::MenuItem("Application"))
			show_application_panel = true;

		if (ImGui::MenuItem("ViewPort"))
			show_viewport_panel = true;

		if (ImGui::MenuItem("Camera Settings"))
			show_camera_settings_panel = true;

		ImGui::Separator();

		if (ImGui::MenuItem("ImGui Demo Window"))
			show_imgui_demo = true;

		if (ImGui::MenuItem("ViewPort Debugging"))
			show_viewport_debug_panel = true;

		ImGui::EndMenu();
	}
}

void PELayer::MainMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Project...", "Ctrl+O"))
			{
				//NewProject();
			}

			if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
			{
				ProjectManager::OpenFile("PaperEngine Project(*.peproj)\0 * .peproj\0");
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Save Project", "Ctrl+S"))
			{
				//SaveProject();
			}

			if (ImGui::MenuItem("Save Project As...", "Ctrl+Shift+S"))
			{
				//SaveProjectAs();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit"))
				Application::GetInstance()->Exit();

			ImGui::EndMenu();
		}

		MenuTabView();

		if (ImGui::BeginMenu("Add"))
		{

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("New Scene"))
			{
				scene = MakeShr<Scene>();
			}

			if (ImGui::MenuItem("Open Scene"))
			{
				std::filesystem::path path = ProjectManager::OpenFile("");
				//std::filesystem::path path = ProjectManager::OpenFile("PaperEngine Scene(*.pescene)\0 * .pescene\0");
				if (!path.empty())
					new_scene = YAMLSerializer::SceneDeserialize(path);
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Save Scene"))
			{
				//YAMLSerializer::SceneSerialize(scene->)
			}

			if (ImGui::MenuItem("Save Scene As..."))
			{

			}

			ImGui::EndMenu();
		}





		ImGui::EndMenuBar();
	}
}