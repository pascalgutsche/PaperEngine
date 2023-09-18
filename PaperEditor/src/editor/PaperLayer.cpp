#include "Editor.h"
#include "PaperLayer.h"

#include "DockManager.h"
#include "SelectionManager.h"

#include "ViewPort.h"

#include "ImGuizmo/ImGuizmo.h"
#include "panels/ContentBrowserPanel.h"
#include "panels/DebuggingPanel.h"
#include "panels/OutlinerPanel.h"
#include "panels/PropertiesPanel.h"
#include "project/ProjectSerializer.h"

#include "renderer/Renderer2D.h"
#include "scripting/ScriptEngine.h"
#include "utils/FileSystem.h"

#include <imgui/misc/cpp/imgui_stdlib.h>

#include "panels/ScenePanel.h"
#include "panels/SettingsPanel.h"


#define NEW_PROJECT_POPUP_ID "NewProjectPopup"

PaperLayer::PaperLayer()
{
	viewports.emplace_back("1");
	viewports.emplace_back("2");
	viewports.emplace_back("3");
	viewports.emplace_back("4");

	//new_scene = YAMLSerializer::SceneDeserialize("assets/scenes/bunker.pescene");
	
}

PaperLayer::~PaperLayer()
{
}

void PaperLayer::OnAttach()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowMenuButtonPosition = ImGuiDir_None;

	panelManager.paperLayer = this;

	PanelManager::SetPanelOpenSetting(PanelOpenSetting::None);
	panelManager.AddPanel<SettingsPanel>("Settings", false);

	PanelManager::SetPanelOpenSetting(PanelOpenSetting::View);
	panelManager.AddPanel<ImGuiDemoPanel>("ImGui Demo", false);
	panelManager.AddPanel<SceneDebuggingPanel>("Scene Debugger", false);
	panelManager.AddPanel<CameraSettingsPanel>("Camera Debugger", false);
	panelManager.AddPanel<ViewportDebuggingPanel>("Viewport Debugger", false);
	panelManager.AddPanel<ApplicationPanel>(true, DockLoc::Right);
	panelManager.AddPanel<ContentBrowserPanel>("Content Browser", true, DockLoc::Bottom);
	panelManager.AddPanel<ScenePanel>("Scene", true, DockLoc::Right);
	panelManager.AddPanel<OutlinerPanel>("Outliner", true, DockLoc::Right);
	panelManager.AddPanel<PropertiesPanel>("Properties", true, DockLoc::RightBottom);

	//TODO: implement proper startup project
	OpenProject("Sandbunker");
	if (!Project::GetActive())
		EmptyProject();

	

}

void PaperLayer::OnDetach()
{
}

void PaperLayer::Update(const float dt)
{
#ifndef DISABLE_SCRIPT_ENGINE
	if (ScriptEngine::ShouldReloadAssemblies())
	{
		ScriptEngine::ReloadAssemblies();
	}
#endif
}

void PaperLayer::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& e)
	{
		if (e.GetButton() == MouseButton::BUTTON_RIGHT)
		{
			for (auto& port : viewports)
			{
				if (port.viewport_hovered)
				{
					for (auto& port1 : viewports)
					{
						port1.viewport_active = false;
						port1.last_viewport_active = false;
					}
					Application::GetWindow()->CursorEnabled(false);
					ImGui::SetWindowFocus(port.name.c_str());
					port.viewport_active = true;
				}
			}
		}
		
		return false;
	});
	dispatcher.dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent& e)
	{
		if (e.GetButton() == MouseButton::BUTTON_RIGHT)
		{
			for (auto& port1 : viewports)
			{
				port1.viewport_active = false;
				port1.last_viewport_active = false;
			}
			Application::GetWindow()->CursorEnabled(true);
		}
		return false;
	});
	dispatcher.dispatch<MouseMovedEvent>([this](MouseMovedEvent& e)
	{
		for (auto& port : viewports)
		{
			if (port.viewport_active)
			{
				port.camera->ControlCamera(e.GetX(), e.GetY(), port.last_viewport_active != port.viewport_active);
				port.last_viewport_active = true;
				break;
			}
		}
		return false;
	});
	dispatcher.dispatch<KeyPressedEvent>([this](KeyPressedEvent& e)
		{

			// Shortcuts
			if (e.GetRepeated())
				return false;

			bool control = Input::IsKeyDown(Key::LEFT_CONTROL) || Input::IsKeyDown(Key::RIGHT_CONTROL);
			bool shift = Input::IsKeyDown(Key::LEFT_SHIFT) || Input::IsKeyDown(Key::RIGHT_SHIFT);

			switch (e.GetKeyCode())
			{
				// Gizmos
				case Key::Q:
				{
					if (GetViewportFocused() && !ImGuizmo::IsUsing())
						gizmo_type = -1;
					break;
				}
				case Key::W:
				{
					if (GetViewportFocused() && !ImGuizmo::IsUsing() && !AnyCameraActive())
						gizmo_type = ImGuizmo::OPERATION::TRANSLATE;
					break;
				}
				case Key::E:
				{
					if (GetViewportFocused() && !ImGuizmo::IsUsing() && !AnyCameraActive())
						gizmo_type = ImGuizmo::OPERATION::ROTATE;
					break;
				}
				case Key::R:
				{
					if (GetViewportFocused() && !ImGuizmo::IsUsing() && !AnyCameraActive())
						gizmo_type = ImGuizmo::OPERATION::SCALE;
					break;
				}
				case Key::S:
				{
					if (e.IsModPressed(Mod::CONTROL) && e.IsModPressed(Mod::SHIFT))
					{
						const std::filesystem::path filePath = FileSystem::SaveFile({ {.name = "Paper Scene", .spec = "pscene"} }, Project::GetAssetScenesPath(), Scene::GetActive()->GetName() + ".pscene");
						SaveSceneAs(filePath);
					}
					else if (e.IsModPressed(Mod::CONTROL))
					{
						SaveScene();
					}
					break;
				}
				case Key::D:
				{
					if (e.IsModPressed(Mod::CONTROL))
					{
						Entity duplicatedEntity = Scene::GetActive()->DuplicateEntity(SelectionManager::GetSelection().ToEntity());
						SelectionManager::Select(duplicatedEntity.GetPaperID());
					}
					break;
				}
				case Key::DELETEKEY:
				{
					if (SelectionManager::HasSelection())
					{
						Entity selectedEntity = Scene::GetActive()->GetEntity(SelectionManager::GetSelection());
						Scene::GetActive()->DestroyEntity(selectedEntity);
						SelectionManager::Deselect();
					}
					break;
				}
			}

			return false;

		});
	

	//if (!viewport_focused)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	event.handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
	//	event.handled |= event.IsInCategory(EventCategoryKeyBoard) & io.WantCaptureKeyboard;
	//}
}

void PaperLayer::EntityDragging()
{
	glm::vec2 mousePos = Input::GetMousPos();
	static glm::vec2 previousMousePos = mousePos;

	static Entity pressedEntity;

	if (Input::IsMouseButtonPressed(MouseButton::BUTTON_LEFT))
	{
		pressedEntity = hovered_entity;
	}

	//drag drop entity
	if (Input::IsMouseButtonDown(MouseButton::BUTTON_LEFT) && mousePos != previousMousePos && hovered_entity == pressedEntity && !drag_entity)
	{
		drag_entity = hovered_entity;
	}

	if (drag_entity && ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceExtern))
	{
		uint64_t entityID = drag_entity.GetPaperID().toUInt64();

		ImGui::SetDragDropPayload("ENTITY_DRAG", &entityID, sizeof(uint64_t));
		ImGui::Text(drag_entity.GetName().c_str());

		ImGui::EndDragDropSource();
	}

	if (Input::IsMouseButtonReleased(MouseButton::BUTTON_LEFT))
	{
		ImGui::ClearDragDrop();
		drag_entity = Entity();
		pressedEntity = Entity();
	}

	previousMousePos = mousePos;
}

void PaperLayer::Imgui(const float dt)
{
	ImGuiDockNodeFlags dockflags = ImGuiDockNodeFlags_PassthruCentralNode;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
	window_flags |= ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;
	ImGuiViewport& viewport = *ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport.Pos);
	ImGui::SetNextWindowSize(viewport.Size);
	ImGui::SetNextWindowViewport(viewport.ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("docking", nullptr, window_flags);
	ImGui::PopStyleVar(3);

	{ static bool createNodes = DockManager::CreateNodes(); }

	DockManager::Update();

	ImGui::DockSpace(DockManager::dockspace_id, ImVec2(0.0f, 0.0f), dockflags);
	UI_MenuBar();
	UI_Toolbar();
	

	ImGui::End();

	panelManager.OnImGuiRender();

	RenderPopups();

	ViewPortPanel();

	MousePicking();

	EntityDragging();


}

void PaperLayer::CreateProject(const std::filesystem::path& projPath, const std::string& projName)
{
	if (projPath.empty() || projName.empty())
	{
		LOG_CORE_ERROR("cannot create project with empty name or path");
		return;
	}

	if (!std::filesystem::exists(projPath))
		std::filesystem::create_directories(projPath);
	else
	{
		LOG_CORE_WARN("cannot create project because it already exists");
		return;
	}

	std::filesystem::copy("resources/ProjectTemplate", projPath, std::filesystem::copy_options::recursive);

	Utils::ReplaceTokenInFile(projPath / "premake5.lua", "$PROJ_NAME$", projName);
	Utils::ReplaceTokenInFile(projPath / "premake5.lua", "$PROJ_PATH$", projPath.string());

	Utils::ReplaceTokenInFile(projPath / "Project.pproj", "$PROJ_NAME$", projName);
	Utils::ReplaceTokenInFile(projPath / "Project.pproj", "$PROJ_PATH$", projPath.string());

	Utils::ReplaceTokenInFile(projPath / "Example.cs", "$PROJ_NAME$", projName);

	std::filesystem::create_directories(projPath / "assets" / "fonts");
	std::filesystem::create_directories(projPath / "assets" / "textures");
	std::filesystem::create_directories(projPath / "assets" / "scenes");
	std::filesystem::create_directories(projPath / "assets" / "scripts" / "Source");

	FileSystem::Move(projPath / "Example.cs", projPath / "assets" / "scripts" / "Source" / "Example.cs");

	GenerateProjectSolution(projPath);

	OpenProject(projPath);
}

void PaperLayer::OpenProject(const std::filesystem::path& projPath)
{
	if (!std::filesystem::exists(projPath))
	{
		LOG_CORE_ERROR("Project '{}' does not exist");
		return;
	}

	std::filesystem::path projectFilePath = projPath;
	if (std::filesystem::is_directory(projectFilePath))
		projectFilePath = projectFilePath / "Project.pproj";

	if (!std::filesystem::exists(projPath))
	{
		LOG_CORE_ERROR("Project '{}' does not exist");
		return;
	}

	Shr<Project> project = ProjectSerializer::Deserialize(projectFilePath);

	OpenProject(project);
}

void PaperLayer::OpenProject(const Shr<Project>& project)
{
	if (Project::GetActive())
		CloseProject();

	Project::SetActive(project);

	panelManager.OnProjectChanged(project);

#ifndef DISABLE_SCRIPT_ENGINE
	if (std::filesystem::exists(Project::GetScriptBinaryFilePath()))
	{
		ScriptEngine::AddAppAssembly(Project::GetScriptBinaryFilePath());
	}
#endif

	OpenScene(Project::GetStartScene());

	ChangeWindowTitle();
}

void PaperLayer::EmptyProject()
{
	const Shr<Project> project = MakeShr<Project>();
	OpenProject(project);
}

void PaperLayer::SaveProject() const
{
	if (!Project::GetActive()) return;

	ProjectSerializer::Serialize(Project::GetActive(), Project::GetProjectPath() / "Project.pproj");
}

void PaperLayer::CloseProject()
{
	if (!editorScene) return;

	SaveProject();

	Scene::SetActive(nullptr);

	ScriptEngine::ResetEngine();

	CORE_ASSERT(editorScene.use_count() == 1, "Something holds reference to editorScene");
	editorScene = nullptr;

	Project::SetActive(nullptr);
}

void PaperLayer::NewScene(const std::string& sceneName)
{
	OpenScene(MakeShr<Scene>(sceneName));
}

void PaperLayer::OpenScene(Shr<Scene> scene)
{
	if (!scene) return;

	if (editorScene)
	{
		CloseScene();
	}

	panelManager.OnSceneChanged(scene);
	editorScene = scene;

	Scene::SetActive(scene);

	ChangeWindowTitle();
	sceneChanged = true;
}

void PaperLayer::CloseScene()
{
	if (!editorScene) return;

	SelectionManager::Deselect();

	OnSceneStop();

	Scene::SetActive(nullptr);

	editorScene = nullptr;
}

void PaperLayer::CloseSceneWithPopup(Shr<Scene> potentialNewScene)
{
	if (!editorScene) return;

	//
	CloseScene();
	if (potentialNewScene)
		OpenScene(potentialNewScene);
	return;
	if (SceneSerializer::IsSceneDirty(editorScene))
	{
		ShowUnsavedScenePopup(potentialNewScene);
		return;
	}
	CloseScene();
	if (potentialNewScene)
		OpenScene(potentialNewScene);
}

void PaperLayer::SaveScene() const
{
	if (!editorScene || editorScene->GetPath().empty()) return;

	SceneSerializer::Serialize(editorScene, Project::GetProjectPath() / editorScene->GetPath());
}

void PaperLayer::SaveSceneAs(const std::filesystem::path& filePath) const
{
	if (!editorScene) return;

	SceneSerializer::Serialize(editorScene, filePath);
}

void PaperLayer::GenerateProjectSolution(const std::filesystem::path& projPath) const
{
	std::string buildPath = projPath.string();
	std::replace(buildPath.begin(), buildPath.end(), '/', '\\'); // Only windows
	buildPath += "\\build.bat";
	system(buildPath.c_str());
}


bool PaperLayer::AnyCameraActive() const
{
	for (const auto& viewPort : viewports)
	{
		if (viewPort.viewport_active)
			return true;
	}
	return false;
}

const ViewPort* PaperLayer::GetViewportFocused() const
{
	for (size_t i = 0; i < viewports.size(); i++)
	{
		if (viewports[i].viewport_focused)
			return &viewports[i];
	}
	return nullptr;
}

void PaperLayer::SwitchScene(const Shr<Scene>& scene)
{
	if (hovered_entity)
		hovered_entity = scene->GetEntity(hovered_entity.GetPaperID());

	Scene::SetActive(scene);
}

void PaperLayer::ShowNewProjectPopup()
{
	ShowPopup("New Project...", [&]()
	{
		static std::string projName;
		static std::string projPath;
		std::string fullProjPath = "";
		if (!projPath.empty())
		{
			fullProjPath = fmt::format("{}\\{}", projPath, projName);
		}

		ImGui::Text("Project is created in:");
		ImGui::Text(fullProjPath.c_str());

		ImGui::InputText("Name: ", &projName);
		ImGui::InputText("Path: ", &projPath);
		ImGui::SameLine();
		if (ImGui::Button("..."))
			projPath = FileSystem::OpenFolder().string();

		std::function<void()> exitFn = [&]()
		{
			projName = "";
			projPath = "";
			ImGui::CloseCurrentPopup();
		};

		if (ImGui::Button("Cancel"))
			exitFn();

		ImGui::SameLine();

		if (ImGui::Button("Create"))
		{
			CreateProject(std::filesystem::path(projPath) / projName, projName);
			exitFn();
		}
	});
}

void PaperLayer::ShowNewScenePopup()
{
	ShowPopup("New Scene...", [&]()
	{
		static std::string sceneName;

		ImGui::InputText("Name: ", &sceneName);

		std::function<void()> exitFn = [&]()
		{
			sceneName = "";
			ImGui::CloseCurrentPopup();
		};

		if (ImGui::Button("Cancel"))
			exitFn();

		ImGui::SameLine();

		if (ImGui::Button("Create"))
		{
			NewScene(sceneName);
			exitFn();
		}
	});
}

void PaperLayer::ShowUnsavedScenePopup(const Shr<Scene>& newScene)
{
	if (unsavedScenePopupCache)
	{
		LOG_WARN("Trying to open UnsavedScenePopup but there is already a scene in the cache!");
		return;
	}
	unsavedScenePopupCache = newScene;
	ShowPopup("Unsaved scene!", [&]()
	{
		bool buttonPressed = false;

		ImGui::TextWrapped("Your current scene has unsaved changes.\nDo you want to save them?");

		if (ImGui::Button("Yes"))
		{
			SaveScene();
			CloseSceneWithPopup();
			buttonPressed = true;
		}
		
		ImGui::SameLine();

		if (ImGui::Button("No"))
		{
			CloseScene();
			buttonPressed = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
			unsavedScenePopupCache = nullptr;
		};

		if (buttonPressed)
		{
			if (unsavedScenePopupCache)
				OpenScene(unsavedScenePopupCache);
			ImGui::CloseCurrentPopup();
			unsavedScenePopupCache = nullptr;
		}
	});
}

void HoverImageButton(const std::string& text)
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
	{
		ImGui::BeginTooltip();
		ImGui::Text(text.c_str());
		ImGui::EndTooltip();
	}
}

bool ToolTipButton(const Shr<Texture>& tex, float size, std::string tooltip = std::string())
{
	bool state = ImGui::ImageButton((void*)tex->GetID(), ImVec2(size, size), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }, 0);
	std::string texName = tooltip.empty() ? tex->GetFilePath().stem().string() : tooltip;
	HoverImageButton(texName);
	return state;
}

void PaperLayer::UI_Toolbar()
{
	static bool first = true;

	const char* name = "##toolbar";
	std::stringstream stream;
	if (first)
		DockManager::DockPanel(name, DockLoc::Top);

	Shr<Texture> playButton = DataPool::GetTexture("resources/editor/viewport/Play.png", true);
	Shr<Texture> simulateButton = DataPool::GetTexture("resources/editor/viewport/Simulate.png", true);
	Shr<Texture> pauseButton = DataPool::GetTexture("resources/editor/viewport/Pause.png", true);
	Shr<Texture> stopButton = DataPool::GetTexture("resources/editor/viewport/Stop.png", true);
	Shr<Texture> stepButton = DataPool::GetTexture("resources/editor/viewport/Step.png", true);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.305f, 0.31f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.1505f, 0.151f, 0.0f));

	ImGui::Begin(name, nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	const float size = ImGui::GetWindowHeight() - 4;
	if (editorScene && Scene::GetActive())
	{
		const bool play = sceneState != SceneState::Simulate;
		const bool simulate = sceneState != SceneState::Play;
		const bool pause = sceneState != SceneState::Edit;

		const Shr<Scene> activeScene = Scene::GetActive();
		const int countButtons = pause && activeScene->IsPaused() ? 3 : 2;
		ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5) - (((size * countButtons) + (ImGui::GetStyle().ItemSpacing.x * countButtons)) * 0.5));
#
		if (pause)
		{
			if (activeScene->IsPaused())
			{
				if (ToolTipButton(playButton, size, "Resume"))
					activeScene->SetPaused(false);
			}
			else
			{

				if (ToolTipButton(pauseButton, size))
					activeScene->SetPaused(true);
			}
		}

		if (play)
		{
			if (pause)
				ImGui::SameLine();

			if (sceneState == SceneState::Edit)
			{
				if (ToolTipButton(playButton, size))
					OnScenePlay();
			}
			else
			{

				if (ToolTipButton(stopButton, size))
					OnSceneStop();
			}
		}

		if (simulate)
		{
			if (play || sceneState != SceneState::Edit)
				ImGui::SameLine();

			if (sceneState == SceneState::Edit)
			{
				if (ToolTipButton(simulateButton, size))
					OnSceneSimulate();
			}
			else
			{
				if (ToolTipButton(stopButton, size))
					OnSceneStop();
			}
		}

		if (pause && activeScene->IsPaused())
		{
			ImGui::SameLine();

			if (ToolTipButton(stepButton, size))
				activeScene->StepFrames();
		}
	}
	else
	{
		ImGui::BeginDisabled();

		ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5) - (((size * 2) + (ImGui::GetStyle().ItemSpacing.x * 2)) * 0.5));

		ToolTipButton(playButton, size);
		ImGui::SameLine();
		ToolTipButton(simulateButton, size);

		ImGui::EndDisabled();
	}
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);

	ImGui::End();


	first = false;
}

void PaperLayer::ChangeWindowTitle() const
{
	std::string title;
	if (Project::GetActive() && Scene::GetActive())
		title = fmt::format("PaperEditor - {0} - {1} - RenderAPI: {2}", Project::GetProjectName(), Scene::GetActive()->GetName(), RenderAPIToString(RenderAPI::GetAPI()));
	else if (Project::GetActive())
		title = fmt::format("PaperEditor - {0} - RenderAPI: {1}", Project::GetProjectName(), RenderAPIToString(RenderAPI::GetAPI()));
	else
		title = fmt::format("PaperEditor - RenderAPI: {0}", RenderAPIToString(RenderAPI::GetAPI()));

	Application::GetInstance()->GetWindow()->SetTitle(title);
}

void PaperLayer::OnScenePlay()
{
	sceneState = SceneState::Play;
	SwitchScene(editorScene->Copy());
	Scene::GetActive()->OnRuntimeStart();
}

void PaperLayer::OnSceneSimulate()
{
	sceneState = SceneState::Simulate;
	SwitchScene(editorScene->Copy());
	Scene::GetActive()->OnSimulationStart();
}

void PaperLayer::OnSceneStop()
{
	if (sceneState == SceneState::Edit) return;
	if (sceneState == SceneState::Play)
		Scene::GetActive()->OnRuntimeStop();
	else if (sceneState == SceneState::Simulate)
		Scene::GetActive()->OnSimulationStop();

	sceneState = SceneState::Edit;
	SwitchScene(editorScene);
}

void PaperLayer::UI_MenuBar()
{

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Project", "Ctrl+O"))
			{
				ShowNewProjectPopup();
			}

			if (ImGui::MenuItem("Open Project", "Ctrl+O"))
			{
				const std::filesystem::path projPath = FileSystem::OpenFile({ {.name = "Paper Project", .spec = "pproj"} });
				OpenProject(projPath);
			}


			if (ImGui::MenuItem("Save Project", "Ctrl+S"))
			{
				SaveProject();
			}

			if (ImGui::MenuItem("Save Project As...", "Ctrl+Shift+S", false, false))
			{
				//SaveProjectAs();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("New Scene"))
			{
				ShowNewScenePopup();
			}

			if (ImGui::MenuItem("Open Scene"))
			{
				const std::filesystem::path filePath = FileSystem::OpenFile({{.name = "Paper Scene", .spec = "pscene"}});
				const Shr<Scene> scene = SceneSerializer::Deserialize(filePath);
				OpenScene(scene);
			}

			if (ImGui::MenuItem("Save Scene"))
			{
				SaveScene();
			}

			if (ImGui::MenuItem("Save Scene As..."))
			{
				const std::filesystem::path filePath = FileSystem::SaveFile({ {.name = "Paper Scene", .spec = "pscene"} }, Project::GetAssetScenesPath(), Scene::GetActive()->GetName() + ".pscene");
				SaveSceneAs(filePath);
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Settings"))
			{
				panelManager.OpenPanel<SettingsPanel>();
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

			if (ImGui::MenuItem("Open Visual-Studio solution"))
			{
				FileSystem::OpenExternal(Project::GetProjectPath() / std::filesystem::path(Project::GetProjectName() + ".sln"));
			}

			ImGui::Separator();

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

		if (ImGui::BeginMenu("_Debug_"))
		{
			if (ImGui::MenuItem("ShowNewProjectPopup()"))
				ShowNewProjectPopup();
			if (ImGui::MenuItem("ShowNewScenePopup()"))
				ShowNewScenePopup();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}