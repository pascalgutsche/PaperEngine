#include "Editor.h"
#include "PELayer.h"

#include <boost/mpl/pair.hpp>

#include "project/ProjectManager.h"



PELayer::PELayer()
	: viewport_size(glm::vec2()), viewport_bounds{glm::vec2(), glm::vec2()}, mouse_pos_viewport_relative(glm::ivec2())
{
	viewports.emplace_back("1");
	//viewports.emplace_back("2");
	//viewports.emplace_back("3");
	//viewports.emplace_back("4");
}

PELayer::~PELayer()
{
}

void PELayer::OnAttach()
{
	scene = YAMLSerializer::SceneDeserialize("bunker.yaml");
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
	mouse_pos_viewport_relative.x = ImGui::GetMousePos().x;
	mouse_pos_viewport_relative.y = ImGui::GetMousePos().y;
	mouse_pos_viewport_relative.x -= viewport_bounds[0].x;
	mouse_pos_viewport_relative.y -= viewport_bounds[0].y - 24;
	glm::vec2 viewportSize = viewport_bounds[1] - viewport_bounds[0];

	mouse_pos_viewport_relative.y = viewportSize.y - mouse_pos_viewport_relative.y;
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
				LOG_DEBUG(port.name);
				port.camera->ControlCamera(e.GetX(), e.GetY(), port.last_viewport_focused != port.viewport_focused);
				port.last_viewport_focused = true;
				break;
			}
		}
		return false;
	});
	

	if (!viewport_focused)
	{
		ImGuiIO& io = ImGui::GetIO();
		event.handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
		event.handled |= event.IsInCategory(EventCategoryKeyBoard) & io.WantCaptureKeyboard;
	}
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

	

	static bool first = true;
	ApplicationPanel(dt, first);
	AssetManagerPanel(dt, first);
	//LayerPanel(dt, first);
	CameraPanel(first, dockflags);
	for (auto& port : viewports)
		port.Panel(this);
	//InspectorPanel(dt, first);

	if (first) {
		first = false;
		ImGui::SetWindowFocus("ViewPort: ");
	}
}

void PELayer::CameraPanel(bool first, ImGuiWindowFlags& dock_flags)
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
	window_flags |= ImGuiWindowFlags_NoBackground;// | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;

	const char* name = "ViewPorts: ";

	if (first)
		DockPanel(name, GetDockspaceMain());

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin(name, nullptr, window_flags);
	ImGui::PopStyleVar(3);

	//ImGui::Begin("Cameras", nullptr, window_flags | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove);
	ImGuiID dockspaddce_id = ImGui::GetID("ViewPort");
	ImGui::DockSpace(dockspaddce_id, ImVec2(0.0f, 0.0f), dock_flags);
	CameraBar();
	//ImGui::End();
	ImGui::End();
}

void PELayer::DockPanel(std::string name, ImGuiID dock_id)
{
	if (dockPanelQueue.find(name) == dockPanelQueue.end())
	{
		dockPanelQueue.emplace(name, dock_id);
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

		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("New Scene"))
			{
				//NewProject();
			}

			if (ImGui::MenuItem("Open Scene"))
			{
				//ProjectManager::OpenFile("PaperEngine Project(*.peproj)\0 * .peproj\0");
			}

			ImGui::BeginDisabled();
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
			ImGui::EndDisabled();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Add"))
		{
			
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Camera"))
		{
			if (ImGui::MenuItem("Add Editor Camera"))
			{
				
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void PELayer::CameraBar()
{
	
	if (ImGui::BeginMenuBar())
	{
		
		if (ImGui::BeginMenu("Camera"))
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
		
		if (ImGui::BeginCombo("##1", "Activate Camera"))
		{

			for (auto& port : viewports)
			{
				if (!port.open)
				{
					if (ImGui::Selectable(port.name.c_str()))
					{
						port.open = true;
					}
				}
			}
			ImGui::EndCombo();
		}

		ImGui::EndMenuBar();
	}
}

void PELayer::ApplicationPanel(const float dt, bool first)
{
	const char* name = "Application: ";
	std::stringstream stream;
	if (first)
		DockPanel(name, GetDockspaceRight());

	ImGui::Begin(name);
	

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
}

static bool displayIcon(const std::filesystem::directory_entry& item, const float size)
{
	if (item.is_directory())
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		if (ImGui::ImageButton((void*)DataPool::GetTexture("folder_icon.png")->GetID(), ImVec2(size, size), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }, 0))
		{
			ImGui::PopStyleColor();
			return true;
		}
		ImGui::PopStyleColor();
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		if (ImGui::ImageButton((void*)DataPool::GetTexture("file_icon.png")->GetID(), ImVec2(size, size), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }, 0))
		{
			ImGui::PopStyleColor();
			return true;
		}
		ImGui::PopStyleColor();
	}
	return false;
}

static void displayText(std::vector<std::string>& filenames)
{
	ImGui::TableNextRow();
	int i = 0;
	for (std::string name : filenames)
	{
		ImGui::TableSetColumnIndex(i);
		ImGui::TextWrapped(filenames.at(i).c_str());
		i++;
	}
}

void PELayer::AssetManagerPanel(const float dt, bool first)
{
	ImGui::ShowDemoWindow();
	static float size = 120.0f;
	static std::filesystem::path path = "assets/";

	std::filesystem::path itemPath;
	bool itemClicked = false;

	const char* name = "Asset Manager: ";
	std::stringstream stream;

	if (first)
		DockPanel(name, GetDockspaceBottom());

	const ImGuiTableFlags flags = ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX;

	ImGui::Begin(name);
	std::stringstream ss;
	for (const auto& pathPart : path)
	{
		if (pathPart.string().empty()) continue;
		ss << pathPart.string() << "/";
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.5, 3));
		if (ImGui::Button(pathPart.filename().string().c_str()))
		{
			path = ss.str();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
			break;
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::SameLine();
		ImGui::Text("/");
		ImGui::SameLine();
	}
	ImGui::Text("");

	int cols = size > (ImGui::GetContentRegionAvail().x) ? 1 : ((int)ImGui::GetContentRegionAvail().x) / size;
	if (cols > 64) cols = 64;

	if (ImGui::BeginTable("dirTable", cols, flags, ImVec2(cols*size, 0.0f)))
	{
		ImGui::TableSetupColumn("dirCol");

		ImGui::TableNextRow();
		std::vector<std::string> filenames;

		while (!exists(path))
		{
			path = path.parent_path();
			if (path == "assets") break;
		}
		int i = 0;
		for (const auto item : std::filesystem::directory_iterator(path))
		{
			if (i >= cols)
			{
				i = 0;

				displayText(filenames);
				ImGui::TableNextRow();
				filenames.clear();
			}
			ImGui::TableSetColumnIndex(i);

			ImGui::PushID(item.path().filename().string().c_str());

			if (displayIcon(item, size))
			{
				if (item.is_directory())
					path = item.path();
				else
				{
					itemPath = path.string() + "/" + item.path().filename().string();
					itemClicked = true;
				}
			}

			ImGui::PopID();

			filenames.push_back(item.path().filename().string());
			
			i++;
		}

		displayText(filenames);
		ImGui::EndTable();
	}
	
	if (itemClicked)
	{
		LOG_TRACE(itemPath);
	}

	ImGui::End();
}

void PELayer::LayerPanel(const float dt, bool first)
{
	const char* name = "Layers: ";
	std::stringstream stream;

	if (first)
		DockPanel(name, GetDockspaceLeft());

	ImGui::Begin(name);

	//for (Layer* layer : Application::GetLayerStack())
	//{
	//	if (layer == &Application::GetImGuiLayer()) continue;
	//	std::vector<Entity*> gameobjects = layer->GetEntitys();
	//	std::vector<Entity*> entities = Application::GetActiveScene()->GetEntitys();
	//	std::vector<UIObject*> uiObjects = layer->GetUIObjects();
	//
	//	if (ImGui::TreeNode(layer->GetName().c_str()))
	//	{
	//		if (ImGui::TreeNode("UIObjects: "))
	//		{
	//			for (int i = 0; i < uiObjects.size(); i++)
	//			{
	//				//if (ImGui::Selectable((uiObjects[i]->GetName() + std::string("##" + std::to_string(i))).c_str(), uiObjects[i] == selectedObject)) {
	//				//	//selectedObject = uiObjects[i];
	//				//}
	//			}
	//			ImGui::TreePop();
	//		}
	//		if (ImGui::TreeNode("Entitys: "))
	//		{
	//			int b = 0;
	//			for (int i = 0; i < gameobjects.size(); i++)
	//			{
	//				if (ImGui::Selectable((gameobjects[i]->GetName() + std::string(" (ObjectID = " + gameobjects[i]->GetUUID().toString() + std::string(")")) + std::string("##" + std::to_string(i))).c_str(), gameobjects[i] == selectedObject)) {
	//					selectedObject = gameobjects[i];
	//				}
	//				b = i;
	//			}
	//			for (int i = 0; i < entities.size(); i++)
	//			{
	//				if (ImGui::Selectable((entities[i]->GetName() + std::string(" (ObjectID = " + entities[i]->GetUUID().toString() + std::string(")")) + std::string("##" + std::to_string(i + b))).c_str(), entities[i] == selectedObject)) {
	//					selectedObject = entities[i];
	//				}
	//			}
	//			ImGui::TreePop();
	//		}
	//
	//		ImGui::Text("");
	//		ImGui::TreePop();
	//	}
	//}

	ImGui::End();
}



void PELayer::InspectorPanel(const float dt, bool first) {
	const char* name = "Inspector: ";
	std::stringstream stream;

	if (first)
		DockPanel(name, GetDockspaceLeftBottom());

	//ImGui::Begin(name);
	//if (dynamic_cast<Entity*>(selectedObject) != nullptr) {
	//	dynamic_cast<Entity*>(selectedObject)->Imgui(dt);
	//}
	//ImGui::End();
}

static void CameraMovement(const Shr<EditorCamera>& camera)
{
	const float dt = Application::GetDT();
	if (Input::IsKeyPressed(KEY_W))
	{
		camera->position.x += 5 * dt * camera->GetFront().x;
		camera->position.z += 5 * dt * camera->GetFront().z;
	}
	if (Input::IsKeyPressed(KEY_A))
	{
		camera->position.x += 5 * dt * camera->GetFront().z;
		camera->position.z -= 5 * dt * camera->GetFront().x;

	}
	if (Input::IsKeyPressed(KEY_S))
	{
		camera->position.x -= 5 * dt * camera->GetFront().x;
		camera->position.z -= 5 * dt * camera->GetFront().z;

	}
	if (Input::IsKeyPressed(KEY_D))
	{
		camera->position.x -= 5 * dt * camera->GetFront().z;
		camera->position.z += 5 * dt * camera->GetFront().x;

	}
	if (Input::IsKeyPressed(KEY_E))
		camera->position.y += 5 * dt;
	if (Input::IsKeyPressed(KEY_Q))
		camera->position.y -= 5 * dt;
}

void ViewPort::Panel(PELayer* peLayer)
{
	if (!open) return;
	if (FramebufferSpecification spec = framebuffer->GetSpecification();
		viewport_size.x > 0.0f && viewport_size.y > 0.0f && // zero sized framebuffer is invalid
		(spec.width != viewport_size.x || spec.height != viewport_size.y))
	{
		framebuffer->Resize((uint32_t)viewport_size.x, (uint32_t)viewport_size.y);

		camera->aspect_ratio = viewport_size.x / viewport_size.y;
	}

	framebuffer->Bind();

	RenderCommand::ClearColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	RenderCommand::Clear();
	framebuffer->ClearAttachment(1, 0);

	peLayer->scene->Render(camera);


	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin(name.c_str(), &open, ImGuiWindowFlags_NoCollapse);

	//viewport_focused = ImGui::IsWindowFocused();
	viewport_hovered = ImGui::IsWindowHovered();

	if (viewport_focused)
		CameraMovement(camera);

	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	viewport_bounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	viewport_bounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

	ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
	viewport_size = { viewport_panel_size.x, viewport_panel_size.y };

	uint32_t textureID = framebuffer->GetColorID(0);
	ImGui::Image((void*)textureID, ImVec2(viewport_size.x, viewport_size.y), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	ImGui::End();
	ImGui::PopStyleVar();


	framebuffer->Unbind();
}
