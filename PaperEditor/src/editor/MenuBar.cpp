#include "Editor.h"

#include "WindowsOpen.h"

#include "PaperLayer.h"
#include "project/ProjectManager.h"
#include "scripting/ScriptEngine.h"

void MenuItemPanel(const char* name, const char* shortcut, bool* p_open)
{
	if (ImGui::MenuItem(name, shortcut, *p_open))
	{
		*p_open = !*p_open;
	}
}

void MenuTabView()
{
	if (ImGui::BeginMenu("View"))
	{
		MenuItemPanel("Asset Manager", nullptr, &show_asset_manager_panel);

		MenuItemPanel("Application", nullptr, &show_application_panel);

		MenuItemPanel("ViewPort", nullptr, &show_viewport_panel);

		MenuItemPanel("Camera Settings", nullptr, &show_camera_settings_panel);

		MenuItemPanel("Outliner", nullptr, &show_outliner_panel);

		MenuItemPanel("Properties", nullptr, &show_property_panel);

		ImGui::Separator();

		MenuItemPanel("ImGui Demo Window", nullptr, &show_imgui_demo);

		MenuItemPanel("ViewPort Debugging", nullptr, &show_viewport_debug_panel);

		MenuItemPanel("Scene Debugger", nullptr, &show_scene_debugger_panel);


		ImGui::EndMenu();
	}
}

void PaperLayer::MainMenuBar()
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
				//ProjectManager::OpenFile("Image Files (*.png, *.jpg)\0*.png;*.jpg;\0");
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

			if (ImGui::MenuItem("Restart"))
				Application::GetInstance()->Exit(true);

			ImGui::EndMenu();
		}

		MenuTabView();

		if (ImGui::BeginMenu("Scripting"))
		{
			if (ImGui::MenuItem("Reload C# Assembly"))
			{
				ScriptEngine::ReloadAssemblies();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Add"))
		{
			if (ImGui::MenuItem("Empty Entity"))
				activeScene->CreateEntity("Entity");
			if (ImGui::MenuItem("Sprite"))
				activeScene->CreateEntity("Sprite").AddComponent<SpriteComponent>();
			if (ImGui::MenuItem("Line"))
				activeScene->CreateEntity("Line").AddComponent<LineComponent>();
			if (ImGui::MenuItem("Text"))
				activeScene->CreateEntity("Text").AddComponent<TextComponent>();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("New Scene"))
			{
				activeScene = MakeShr<Scene>();
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
				YAMLSerializer::SceneSerialize(activeScene->GetPath(), activeScene);
			}

			if (ImGui::MenuItem("Save Scene As..."))
			{

			}


			

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}