#include "Editor.h"
#include "PaperLayer.h"

#include "DockManager.h"
#include "SelectionManager.h"

#include "ViewPort.h"

#include "ImGuizmo/ImGuizmo.h"
#include "panels/AssetManagerPanel.h"
#include "panels/DebuggingPanel.h"
#include "panels/OutlinerPanel.h"
#include "panels/ProjectPanel.h"
#include "panels/PropertiesPanel.h"
#include "project/ProjectSerializer.h"

#include "renderer/Renderer2D.h"
#include "scripting/ScriptEngine.h"
#include "utils/FileSystem.h"


PaperLayer::PaperLayer()
{
	viewports.emplace_back("1");
	viewports.emplace_back("2");
	viewports.emplace_back("3");
	viewports.emplace_back("4");

	new_scene = YAMLSerializer::SceneDeserialize("assets/scenes/bunker.pescene");
	
}

PaperLayer::~PaperLayer()
{
}

void PaperLayer::OnAttach()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowMenuButtonPosition = ImGuiDir_None;

	panelManager.paperLayer = this;

	panelManager.AddPanel<ImGuiDemoPanel>("ImGui Demo", false);

	panelManager.AddPanel<SceneDebuggingPanel>("Scene Debugger", false);
	panelManager.AddPanel<CameraSettingsPanel>("Camera Debugger", false);
	panelManager.AddPanel<ViewportDebuggingPanel>("Viewport Debugger", false);
	panelManager.AddPanel<ApplicationPanel>(true, DockLoc::Right);
	panelManager.AddPanel<AssetManagerPanel>("Asset Manager", true, DockLoc::Bottom);
	panelManager.AddPanel<OutlinerPanel>("Outliner", true, DockLoc::Right);
	panelManager.AddPanel<PropertiesPanel>("Properties", true, DockLoc::RightBottom);
	panelManager.AddPanel<NewProjectPanel>("New Project...", false);

	ProjectSerializer::Serialize(MakeShr<Project>(), "resources/ProjectTemplate/Project.pproj");
}

void PaperLayer::OnDetach()
{
}



void PaperLayer::Update(const float dt)
{
	if (ScriptEngine::ShouldReloadAssemblies())
	{
		//if (sceneState != SceneState::Edit)
		//	OnSceneStop();
		ScriptEngine::ReloadAssemblies();
	}

	CheckSceneChange();

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
	MainMenuBar();

	ImGui::End();

	panelManager.OnImGuiRender();

	ToolbarPanel();
	ViewPortPanel();

	MousePicking();

	EntityDragging();


}

void PaperLayer::CreateProject(const std::filesystem::path& projPath, const std::string& projName)
{
	if (!std::filesystem::exists(projPath))
		std::filesystem::create_directories(projPath);


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

void PaperLayer::CheckSceneChange()
{
	const Shr<Scene> activeScene = Scene::GetActive();
	if (!new_scene) return;

	if (sceneState != SceneState::Edit)
		OnSceneStop();

	if (!activeScene || !activeScene->IsDirty())
	{
		editorScene = new_scene;
		SwitchScene(editorScene);
		new_scene = nullptr;
		return;
	}

	const ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowSize(ImVec2(300.0f, 300.0f), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f + Application::GetWindow()->GetPosition().x, io.DisplaySize.y * 0.5f + Application::GetWindow()->GetPosition().y), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::Begin("UNSAVED SCENE", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	ImGui::TextWrapped("Do you want to save the current scene?");
	if (ImGui::Button("Yes"))
	{
		if (activeScene->GetPath().empty())
		{
			activeScene->SetPath(FileSystem::SaveFile());
		}
		std::string filePath = activeScene->GetPath().string();
		if (!filePath.empty())
		{
			activeScene->SetPath(filePath);
			YAMLSerializer::SceneSerialize(filePath, activeScene);
			activeScene->SetClean();
			editorScene = new_scene;
			SwitchScene(editorScene);
			new_scene = nullptr;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("No"))
	{
		editorScene = new_scene;
		SwitchScene(editorScene);
		new_scene = nullptr;
	}
	ImGui::End();
}

void HoverImageButton(std::string text)
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

void PaperLayer::ToolbarPanel()
{
	static bool first = true;

	const char* name = "##toolbar";
	std::stringstream stream;
	if (first)
		DockManager::DockPanel(name, DockLoc::Top);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.305f, 0.31f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.1505f, 0.151f, 0.0f));

	ImGui::Begin(name, nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	if (!editorScene)
		ImGui::BeginDisabled();

	Shr<Texture> playButton = DataPool::GetTexture("resources/editor/viewport/Play.png", true);
	Shr<Texture> simulateButton = DataPool::GetTexture("resources/editor/viewport/Simulate.png", true);
	Shr<Texture> pauseButton = DataPool::GetTexture("resources/editor/viewport/Pause.png", true);
	Shr<Texture> stopButton = DataPool::GetTexture("resources/editor/viewport/Stop.png", true);
	Shr<Texture> stepButton = DataPool::GetTexture("resources/editor/viewport/Step.png", true);

	const bool play = sceneState != SceneState::Simulate;
	const bool simulate = sceneState != SceneState::Play;
	const bool pause = sceneState != SceneState::Edit;

	const Shr<Scene> activeScene = Scene::GetActive();
	const int countButtons = pause && activeScene->IsPaused() ? 3 : 2;
	const float size = ImGui::GetWindowHeight() - 4;
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

	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);

	if (!editorScene)
		ImGui::EndDisabled();

	ImGui::End();


	first = false;
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
	if (sceneState == SceneState::Play)
		Scene::GetActive()->OnRuntimeStop();
	else if (sceneState == SceneState::Simulate)
		Scene::GetActive()->OnSimulationStop();

	sceneState = SceneState::Edit;
	SwitchScene(editorScene);
}