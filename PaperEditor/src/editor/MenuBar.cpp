#include "Editor.h"

#include "PaperLayer.h"
#include "panels/ProjectPanel.h"
#include "project/ProjectManager.h"
#include "scripting/ScriptEngine.h"
#include "utils/FileSystem.h"

void MenuItemPanel(const char* name, const char* shortcut, bool* p_open)
{
	if (ImGui::MenuItem(name, shortcut, *p_open))
	{
		*p_open = !*p_open;
	}
}

void MenuTabView()
{
	
}

void PaperLayer::MainMenuBar()
{
	
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Project...", "Ctrl+O"))
			{
				panelManager.OpenPanel(Utils::TypeToStdString<NewProjectPanel>());
			}

			if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
			{
				std::filesystem::path projPath = FileSystem::OpenFile("", { {.name = "Paper Project", .spec = "pproj"} });
				OpenProject(projPath);
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

		if (ImGui::BeginMenu("Edit"))
		{
			for (PanelData& panelData : panelManager.GetPanels(PanelOpenSetting::Edit) | std::views::values)
			{
				ImGui::MenuItem(panelData.displayName.c_str(), nullptr, &panelData.isOpen);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			for (PanelData& panelData : panelManager.GetPanels(PanelOpenSetting::View) | std::views::values)
			{
				ImGui::MenuItem(panelData.displayName.c_str(), nullptr, &panelData.isOpen);
			}

			ImGui::EndMenu();
		}

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
			const Shr<Scene> activeScene = Scene::GetActive();
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
				Scene::SetActive(MakeShr<Scene>());
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
				const Shr<Scene> activeScene = Scene::GetActive();
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