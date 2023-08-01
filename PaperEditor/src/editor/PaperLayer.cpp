#include "Editor.h"
#include "PaperLayer.h"

#include "WindowsOpen.h"

#include "project/ProjectManager.h"

#include "ViewPort.h"

#include "ImGuizmo/ImGuizmo.h"

#include "renderer/Renderer2D.h"


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
	//style.FrameRounding = 12.0f;
	//style.GrabRounding = 12.0f;
	// scene =
	//Shr<Scene> scene1 = MakeShr<Scene>();
	//Paper::UUID uuid = scene1->CreateEntity("lol").GetUUID();
	//
	//scene1->GetEntity(uuid).AddComponent<SpriteComponent>();
	//scene1->GetEntity(uuid).AddComponent<CircleComponent>();
	//scene1->GetEntity(uuid).AddComponent<LineComponent>();
	//scene1->GetEntity(uuid).AddComponent<TextComponent>();
	//scene1->GetEntity(uuid).AddTag({ "A", "B", "C", "AA", "aB", "Ac" });
	//
	//YAMLSerializer::SceneSerialize("bunker.yaml", scene1);

}

void PaperLayer::OnDetach()
{
}



void PaperLayer::Update(const float dt)
{
	CheckSceneChange();


}

void PaperLayer::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& e)
	{
		if (e.GetButton() == MOUSE_BUTTON_RIGHT)
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
		if (e.GetButton() == MOUSE_BUTTON_LEFT && false)
		{
			if (hovered_entity == active_entity && gizmo_type == -1) 
			{
				active_entity = Entity();
				LOG_DEBUG(11);
			}
			else if (!ImGuizmo::IsUsing()) 
			{
				active_entity = hovered_entity;
				LOG_DEBUG(22);
			}
		}
		
		return false;
	});
	dispatcher.dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent& e)
	{
		if (e.GetButton() == MOUSE_BUTTON_RIGHT)
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
			if (e.getRepeated())
				return false;

			bool control = Input::IsKeyPressed(KEY_LEFT_CONTROL) || Input::IsKeyPressed(KEY_RIGHT_CONTROL);
			bool shift = Input::IsKeyPressed(KEY_LEFT_SHIFT) || Input::IsKeyPressed(KEY_RIGHT_SHIFT);

			switch (e.getKeyCode())
			{
				// Gizmos
				case KEY_Q:
				{
					if (GetViewportFocused() && !ImGuizmo::IsUsing())
						gizmo_type = -1;
					break;
				}
				case KEY_W:
				{
					if (GetViewportFocused() && !ImGuizmo::IsUsing() && !AnyCameraActive())
						gizmo_type = ImGuizmo::OPERATION::TRANSLATE;
					break;
				}
				case KEY_E:
				{
					if (GetViewportFocused() && !ImGuizmo::IsUsing() && !AnyCameraActive())
						gizmo_type = ImGuizmo::OPERATION::ROTATE;
					break;
				}
				case KEY_R:
				{
					if (GetViewportFocused() && !ImGuizmo::IsUsing() && !AnyCameraActive())
						gizmo_type = ImGuizmo::OPERATION::SCALE;
					break;
				}
				case KEY_DELETE:
				{
					if (active_entity)
					{
						activeScene->DestroyEntity(active_entity);
						active_entity = Entity();
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

	dockspace_id = ImGui::GetID("dockspace");

	
	if (first_frame)
	{
		first_frame = false;
		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, ImVec2(Application::GetWindow()->GetWidth() + 500, Application::GetWindow()->GetHeight() + 500));

		dock_id_main = dockspace_id;

		dock_id_right = ImGui::DockBuilderSplitNode(dock_id_main,       ImGuiDir_Right, 0.15f, nullptr, &dock_id_main);
		dock_id_right_bottom = ImGui::DockBuilderSplitNode(dock_id_right,ImGuiDir_Down, 0.5f, nullptr, &dock_id_right);
		dock_id_left = ImGui::DockBuilderSplitNode(dock_id_main,        ImGuiDir_Left, 0.2f, nullptr, &dock_id_main);
		dock_id_down = ImGui::DockBuilderSplitNode(dock_id_main,        ImGuiDir_Down, 0.15f, nullptr, &dock_id_main);
		dock_id_right2 = ImGui::DockBuilderSplitNode(dock_id_right,     ImGuiDir_Left, 0.2f, nullptr, &dock_id_right);
		dock_id_left_bottom = ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Down, 0.5f, nullptr, &dock_id_left);
		dock_id_up = ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Up, 0.04f, nullptr, &dock_id_main);

		// Disable tab bar for custom toolbar
		ImGuiDockNode* node = ImGui::DockBuilderGetNode(dock_id_main);
		node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

		node = ImGui::DockBuilderGetNode(dock_id_up);
		node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

		ImGui::DockBuilderFinish(dockspace_id);
	}
	for (auto [key, val] : dockPanelQueue)
	{
		ImGui::DockBuilderDockWindow(key.c_str(), val);
	}
	dockPanelQueue.clear();

	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockflags);
	MainMenuBar();


	ImGui::End();

	if (show_imgui_demo)
		ImGui::ShowDemoWindow(&show_imgui_demo);


	if (show_asset_manager_panel)
		AssetManagerPanel();

	if (show_application_panel)
		ApplicationPanel();

	if (show_outliner_panel)
		OutlinerPanel();

	if (show_property_panel)
		PropertiesPanel();

	if (show_camera_settings_panel)
		CameraSettingsPanel();

	if (show_viewport_debug_panel)
		ViewPortDebugging();

	if (show_scene_debugger_panel)
		SceneDebugger();

	ToolbarPanel();
	ViewPortPanel();

	MousePicking();

}



void PaperLayer::DockPanel(std::string name, ImGuiID dock_id)
{
	if (dockPanelQueue.find(name) == dockPanelQueue.end())
	{
		dockPanelQueue.emplace(name, dock_id);
	}
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
	if (active_entity)
		active_entity = scene->GetEntity(active_entity.GetUUID());

	if (hovered_entity)
		hovered_entity = scene->GetEntity(hovered_entity.GetUUID());

	activeScene = scene;
}

void PaperLayer::CheckSceneChange()
{
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
			activeScene->SetPath(ProjectManager::SaveFile(""));
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

void PaperLayer::ToolbarPanel()
{
	static bool first = true;

	const char* name = "##toolbar";
	std::stringstream stream;
	if (first)
		DockPanel(name, dock_id_up);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.305f, 0.31f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.1505f, 0.151f, 0.0f));

	ImGui::Begin(name, nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	void* runtimePlayButton = (void*)DataPool::GetTexture("resources/editor/viewport/RuntimePlay.png", true)->GetID();
	void* simulatePlayButton = (void*)DataPool::GetTexture("resources/editor/viewport/SimulatePlay.png", true)->GetID();
	void* pauseButton = (void*)DataPool::GetTexture("resources/editor/viewport/Pause.png", true)->GetID();
	void* stopButton = (void*)DataPool::GetTexture("resources/editor/viewport/Stop.png", true)->GetID();



	float size = ImGui::GetWindowHeight() - 4;
	ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5) - (size + (ImGui::GetStyle().ItemSpacing.x * 0.5)));

	bool (*imageButton)(void* image, std::string tooltip) = [](void* image, std::string tooltip)
	{
		float size = ImGui::GetWindowHeight() - 4;
		bool state =  ImGui::ImageButton(image, ImVec2(size, size), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }, 0);
		HoverImageButton(tooltip);
		return state;
	};

	switch(sceneState)
	{
	case SceneState::Edit:
		if (imageButton(runtimePlayButton, "Play"))
			OnScenePlay();

		ImGui::SameLine();

		if (imageButton(simulatePlayButton, "Simulate"))
			OnSceneSimulate();

		break;
	case SceneState::RuntimePlay:
		if (imageButton(pauseButton, "Pause"))
			sceneState = SceneState::RuntimePause;
		
		ImGui::SameLine();

		if (imageButton(stopButton, "Stop"))
			OnSceneStop();

		break;
	case SceneState::RuntimePause:
		if (imageButton(runtimePlayButton, "Resume"))
			sceneState = SceneState::RuntimePlay;

		ImGui::SameLine();

		if (imageButton(stopButton, "Stop"))
			OnSceneStop();

		break;
	case SceneState::SimulatePlay:
		if (imageButton(pauseButton, "Pause"))
			sceneState = SceneState::SimulatePause;

		ImGui::SameLine();
		if (imageButton(stopButton, "Stop"))
			OnSceneStop();

		break;
	case SceneState::SimulatePause:
		if (imageButton(simulatePlayButton, "Resume"))
			sceneState = SceneState::SimulatePlay;

		ImGui::SameLine();
		if (imageButton(stopButton, "Stop"))
			OnSceneStop();

		break;
	}


	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);

	ImGui::End();


	first = false;
}

void PaperLayer::OnScenePlay()
{
	sceneState = SceneState::RuntimePlay;
	SwitchScene(editorScene->Copy());
}

void PaperLayer::OnSceneSimulate()
{
	sceneState = SceneState::SimulatePlay;
}

void PaperLayer::OnScenePause()
{
	if (sceneState == SceneState::RuntimePlay)
		sceneState = SceneState::RuntimePause;

	if (sceneState == SceneState::SimulatePlay)
		sceneState = SceneState::SimulatePause;
}

void PaperLayer::OnSceneStop()
{
	sceneState = SceneState::Edit;
	SwitchScene(editorScene);
}