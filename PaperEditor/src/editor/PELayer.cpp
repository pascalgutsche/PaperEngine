#include "Editor.h"
#include "PELayer.h"

#include "WindowsOpen.h"

#include "project/ProjectManager.h"

#include "ViewPort.h"

#include "ImGuizmo/ImGuizmo.h"


PELayer::PELayer()
{
	viewports.emplace_back("1");
	viewports.emplace_back("2");
	viewports.emplace_back("3");
	viewports.emplace_back("4");
}

PELayer::~PELayer()
{
}

void PELayer::OnAttach()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowMenuButtonPosition = ImGuiDir_None;
	style.FrameRounding = 12.0f;
	style.GrabRounding = 12.0f;
	// scene =
	//Shr<Scene> scene1 = MakeShr<Scene>();
	//ppr::UUID uuid = scene1->CreateEntity("lol").GetUUID();
	//
	//scene1->GetEntity(uuid).AddComponent<SpriteComponent>();
	//scene1->GetEntity(uuid).AddComponent<CircleComponent>();
	//scene1->GetEntity(uuid).AddComponent<LineComponent>();
	//scene1->GetEntity(uuid).AddComponent<TextComponent>();
	//scene1->GetEntity(uuid).AddTag({ "A", "B", "C", "AA", "aB", "Ac" });
	//
	//YAMLSerializer::SceneSerialize("bunker.yaml", scene1);

}

void PELayer::OnDetach()
{
}



void PELayer::Update(const float dt)
{
	

	CheckSceneChange();

	MousePicking();
}

void PELayer::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& e)
	{
		for (auto& port : viewports)
		{
			if (port.viewport_hovered && e.GetButton() == MOUSE_BUTTON_RIGHT)
			{
				for (auto& port1 : viewports)
				{
					port1.viewport_focused = false;
					port1.last_viewport_focused = false;
				}
				Application::GetWindow()->CursorEnabled(false);
				ImGui::SetWindowFocus(port.name.c_str());
				port.viewport_focused = true;
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
				port1.viewport_focused = false;
				port1.last_viewport_focused = false;
			}
			Application::GetWindow()->CursorEnabled(true);
		}
		return false;
	});
	dispatcher.dispatch<MouseMovedEvent>([this](MouseMovedEvent& e)
	{
		for (auto& port : viewports)
		{
			if (port.viewport_focused)
			{
				port.camera->ControlCamera(e.GetX(), e.GetY(), port.last_viewport_focused != port.viewport_focused);
				port.last_viewport_focused = true;
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
					if (!ImGuizmo::IsUsing())
						gizmo_type = -1;
					break;
				}
				case KEY_W:
				{
					if (!ImGuizmo::IsUsing() && !AnyCameraActive())
						gizmo_type = ImGuizmo::OPERATION::TRANSLATE;
					break;
				}
				case KEY_E:
				{
					if (!ImGuizmo::IsUsing() && !AnyCameraActive())
						gizmo_type = ImGuizmo::OPERATION::ROTATE;
					break;
				}
				case KEY_R:
				{
					if (!ImGuizmo::IsUsing() && !AnyCameraActive())
						gizmo_type = ImGuizmo::OPERATION::SCALE;
					break;
				}
				case KEY_DELETE:
				{
					if (active_entity)
					{
						scene->DestroyEntity(active_entity);
						active_entity = Entity();
						hovered_entity = Entity();
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

void PELayer::Imgui(const float dt)
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

	static bool initialized = false;
	if (!initialized)
	{
		initialized = true;
		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, ImVec2(Application::GetWindow()->GetWidth() + 500, Application::GetWindow()->GetHeight() + 500));

		dock_id_main = dockspace_id;
		
		dock_id_right = ImGui::DockBuilderSplitNode(dock_id_main,       ImGuiDir_Right, 0.2f, nullptr, &dock_id_main);
		dock_id_left = ImGui::DockBuilderSplitNode(dock_id_main,        ImGuiDir_Left, 0.2f, nullptr, &dock_id_main);
		dock_id_top = ImGui::DockBuilderSplitNode(dock_id_main,         ImGuiDir_Up  ,  0.2f, nullptr, &dock_id_main);
		dock_id_down = ImGui::DockBuilderSplitNode(dock_id_main,        ImGuiDir_Down, 0.25f, nullptr, &dock_id_main);
		dock_id_right2 = ImGui::DockBuilderSplitNode(dock_id_right,     ImGuiDir_Left, 0.2f, nullptr, &dock_id_right);
		dock_id_left_bottom = ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Down, 0.5f, nullptr, &dock_id_left);
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

	if (show_viewport_panel)
		ViewPortPanel();

	if (show_camera_settings_panel)
		CameraSettingsPanel();

	if (show_viewport_debug_panel)
		ViewPortDebugging();

	if (show_scene_debugger_panel)
		SceneDebugger();
}



void PELayer::DockPanel(std::string name, ImGuiID dock_id)
{
	if (dockPanelQueue.find(name) == dockPanelQueue.end())
	{
		dockPanelQueue.emplace(name, dock_id);
	}
}

bool PELayer::AnyCameraActive() const
{
	for (const auto& viewPort : viewports)
	{
		if (viewPort.viewport_focused)
			return true;
	}
	return false;
}

void PELayer::CheckSceneChange()
{
	if (!new_scene) return;
	if (!scene || !scene->IsDirty())
	{
		scene = new_scene;
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
		if (scene->GetPath().empty())
		{
			std::filesystem::path filePath = ProjectManager::SaveFile("");
			if (!filePath.empty())
			{
				scene->SetPath(filePath);
				YAMLSerializer::SceneSerialize(filePath, scene);
				scene->SetClean();
			}
		}
		scene = new_scene;
		new_scene = nullptr;
	}
	ImGui::SameLine();
	if (ImGui::Button("No"))
	{
		scene = new_scene;
		new_scene = nullptr;
	}
	ImGui::End();
}





void PELayer::ApplicationPanel()
{
	static bool first = true;
	const float dt = Application::GetDT();

	const char* name = "Application: ";
	std::stringstream stream;
	if (first)
		DockPanel(name, GetDockspaceRight());

	ImGui::Begin(name, &show_application_panel);
	

	static float time = 0;
	static float timehelper = -1;
	static float history = 5;
	static int flags = ImPlotAxisFlags_NoTickLabels;
	static ScrollingBuffer sbuff_dt(300);

	if (timehelper >= 0.016f || timehelper == -1) {
		timehelper = 0;
		sbuff_dt.AddPoint(time, 1000 * dt);
	}
	timehelper += dt;
	time += dt;

	std::string entity_name1 = "None";
	if (hovered_entity)
		entity_name1 = hovered_entity.GetName();
	ImGui::Text("Hovered Entity: %s", entity_name1.c_str());

	std::string entity_name2 = "None";
	if (active_entity)
		entity_name2 = active_entity.GetName();
	ImGui::Text("Active Entity: %s", entity_name2.c_str());

	if (ImGui::TreeNode("Time"))
	{
		ImGui::SliderFloat("Time", &history, 1, 5, "%.1f");

		stream << "ms per frame: " << 1000 * dt;
		ImGui::Text(stream.str().c_str()); stream.str("");
		if (ImPlot::BeginPlot("##ms_per_frame", ImVec2(-1, 100))) {
			ImPlot::SetupAxes(NULL, NULL, flags, flags);
			ImPlot::SetupAxisLimits(ImAxis_X1, time - history, time, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 20);
			ImPlot::SetNextFillStyle(ImVec4(0.0f, 0.0f, 1.0f, -1.0f), 0.5f);
			ImPlot::PlotLine("##ms", &sbuff_dt.Data[0].x, &sbuff_dt.Data[0].y, sbuff_dt.Data.size(), 0, sbuff_dt.Offset, 2 * sizeof(float));
			ImPlot::EndPlot();
		}

		stream << "frames per sec: " << 1 / dt;
		ImGui::Text(stream.str().c_str()); stream.str("");

		static bool vsync = true;
		ImGui::Checkbox("V-Sync", &vsync);
		Application::GetWindow()->SetVSync(vsync);



		ImGui::Text("");
		ImGui::TreePop();
	}


	if (ImGui::TreeNode("Render Stats"))
	{
		stream << "Draw calls: " << RenderCommand::GetStats().drawCalls;
		ImGui::BulletText(stream.str().c_str()); stream.str("");

		stream << "Object count: " << RenderCommand::GetStats().objectCount;
		ImGui::BulletText(stream.str().c_str()); stream.str("");

		stream << "Data size: " << RenderCommand::GetStats().dataSize << " Bytes";
		ImGui::BulletText(stream.str().c_str()); stream.str("");

		stream << "Vertex count: " << RenderCommand::GetStats().vertexCount;
		ImGui::BulletText(stream.str().c_str()); stream.str("");

		stream << "Indices count: " << RenderCommand::GetStats().elementCount;
		ImGui::BulletText(stream.str().c_str()); stream.str("");

		ImGui::Text("");

		stream << "Polygon Model: ";
		ImGui::Text(stream.str().c_str()); stream.str("");

		static int selected = 0;
		if (ImGui::Selectable("OFF", selected == 0))
			RenderCommand::SetPolygonModel(Polygon::OFF);
		if (ImGui::Selectable("LINE", selected == 2))
			RenderCommand::SetPolygonModel(Polygon::LINE);
		if (ImGui::Selectable("POINT", selected == 1))
			RenderCommand::SetPolygonModel(Polygon::POINT);



		ImGui::BeginDisabled();
		if (ImGui::TreeNode("Textures in use"))
		{
			/*
			for (Shr<Texture> texture : Renderer2D::GetTexturesInUse())
			{

				ImGui::Selectable(texture->GetName().c_str(), false);

			}
			*/
			ImGui::Text("");
			ImGui::TreePop();
		}
		ImGui::EndDisabled();


		ImGui::TreePop();
	}
	ImGui::BeginDisabled();
	if (ImGui::TreeNode("Camera"))
	{
		//if (ImGui::Button("Reset Camera"))
		//{
		//	Application::GetActiveScene()->GetCamera()->fov = 1.0f;
		//	Application::GetActiveScene()->GetCamera()->position = glm::vec3(0.0f, 0.0f, 5.0f);
		//}
		//
		//glm::vec3 pos = Application::GetActiveScene()->GetCamera()->position;
		//float arrayPos[]{ pos.x, pos.y, pos.z };
		//ImGui::DragFloat3("XYZ", arrayPos, 0.5f);
		//Application::GetActiveScene()->GetCamera()->position = glm::vec3(arrayPos[0], arrayPos[1], arrayPos[2]);
		//
		//float fov = Application::GetActiveScene()->GetCamera()->fov;
		//ImGui::DragFloat("FOV", &fov, 0.5f);
		//Application::GetActiveScene()->GetCamera()->fov = fov;
		//
		ImGui::TreePop();
	}
	ImGui::EndDisabled();

	ImGui::End();

	first = false;
}

void PELayer::SceneDebugger()
{
	ImGui::Begin("SceneDebugger", &show_scene_debugger_panel);

	if (!scene)
	{
		ImGui::Text("no scene active!");
		ImGui::End();
		return;
	}

	auto view = scene->Registry().view<TransformComponent>();

	for (auto [entity, transform] : view.each()) {

		ImGui::Text(Entity(entity, scene.get()).GetName().c_str());
		ImGui::InputFloat3("Position", &transform.position.x);
		ImGui::InputFloat3("Scale", &transform.scale.x);
		ImGui::InputFloat3("Rotation", &transform.rotation.x);

		ImGui::Text("-----------------------");
	}

	ImGui::End();
}



//void PELayer::LayerPanel(const float dt, bool first)
//{
//	const char* name = "Layers: ";
//	std::stringstream stream;
//
//	if (first)
//		DockPanel(name, GetDockspaceLeft());
//
//	ImGui::Begin(name);
//
//	//for (Layer* layer : Application::GetLayerStack())
//	//{
//	//	if (layer == &Application::GetImGuiLayer()) continue;
//	//	std::vector<Entity*> gameobjects = layer->GetEntitys();
//	//	std::vector<Entity*> entities = Application::GetActiveScene()->GetEntitys();
//	//	std::vector<UIObject*> uiObjects = layer->GetUIObjects();
//	//
//	//	if (ImGui::TreeNode(layer->GetName().c_str()))
//	//	{
//	//		if (ImGui::TreeNode("UIObjects: "))
//	//		{
//	//			for (int i = 0; i < uiObjects.size(); i++)
//	//			{
//	//				//if (ImGui::Selectable((uiObjects[i]->GetName() + std::string("##" + std::to_string(i))).c_str(), uiObjects[i] == selectedObject)) {
//	//				//	//selectedObject = uiObjects[i];
//	//				//}
//	//			}
//	//			ImGui::TreePop();
//	//		}
//	//		if (ImGui::TreeNode("Entitys: "))
//	//		{
//	//			int b = 0;
//	//			for (int i = 0; i < gameobjects.size(); i++)
//	//			{
//	//				if (ImGui::Selectable((gameobjects[i]->GetName() + std::string(" (ObjectID = " + gameobjects[i]->GetUUID().toString() + std::string(")")) + std::string("##" + std::to_string(i))).c_str(), gameobjects[i] == selectedObject)) {
//	//					selectedObject = gameobjects[i];
//	//				}
//	//				b = i;
//	//			}
//	//			for (int i = 0; i < entities.size(); i++)
//	//			{
//	//				if (ImGui::Selectable((entities[i]->GetName() + std::string(" (ObjectID = " + entities[i]->GetUUID().toString() + std::string(")")) + std::string("##" + std::to_string(i + b))).c_str(), entities[i] == selectedObject)) {
//	//					selectedObject = entities[i];
//	//				}
//	//			}
//	//			ImGui::TreePop();
//	//		}
//	//
//	//		ImGui::Text("");
//	//		ImGui::TreePop();
//	//	}
//	//}
//
//	ImGui::End();
//}



//void PELayer::InspectorPanel(const float dt, bool first) {
//	const char* name = "Inspector: ";
//	std::stringstream stream;
//
//	if (first)
//		DockPanel(name, GetDockspaceLeftBottom());
//
//	//ImGui::Begin(name);
//	//if (dynamic_cast<Entity*>(selectedObject) != nullptr) {
//	//	dynamic_cast<Entity*>(selectedObject)->Imgui(dt);
//	//}
//	//ImGui::End();
//}




