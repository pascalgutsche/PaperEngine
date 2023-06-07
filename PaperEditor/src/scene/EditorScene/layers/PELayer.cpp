#include "Editor.h"
#include "PELayer.h"

#include "project/ProjectManager.h"

PELayer::PELayer()
	: viewportSize(glm::vec2()), viewportBounds{glm::vec2(), glm::vec2()}, mousePosViewportRelative(glm::ivec2())
{
}

PELayer::~PELayer()
{
}

void PELayer::OnAttach()
{
    FramebufferSpecification spec;
    spec.attachment = { FramebufferTexFormat::RGBA8, FramebufferTexFormat::RED_INTEGER, FramebufferTexFormat::Depth };
    spec.width = Application::GetWindow()->GetWidth();
    spec.height = Application::GetWindow()->GetHeight();
    framebuffer = Framebuffer::CreateBuffer(spec);
}

void PELayer::OnDetach()
{
}

void PELayer::Update(const float dt)
{
    // Resize
    if (FramebufferSpecification spec = framebuffer->GetSpecification();
        viewportSize.x > 0.0f && viewportSize.y > 0.0f && // zero sized framebuffer is invalid
        (spec.width != viewportSize.x || spec.height != viewportSize.y))
    {
        framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);

        //update cameras aspect ratio //TODO: nuke it somewhere else
        Application::GetActiveScene()->GetCamera()->aspectRatio = viewportSize.x / viewportSize.y;
    }

    mousePosViewportRelative = *(glm::vec2*)&ImGui::GetMousePos();
    mousePosViewportRelative.x -= viewportBounds[0].x;
    mousePosViewportRelative.y -= viewportBounds[0].y - 24;
    glm::vec2 viewportSize = viewportBounds[1] - viewportBounds[0];

    mousePosViewportRelative.y = viewportSize.y - mousePosViewportRelative.y;

    //render
    framebuffer->Bind();
    RenderCommand::Clear();
    framebuffer->ClearAttachment(1, 0);

    scene->Render();

    framebuffer->Unbind();
}

void PELayer::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
    dispatcher.dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& e)
    {
        if (viewportHovered && e.GetButton() == MOUSE_BUTTON_RIGHT)
        {
            cameraControlMode = true;
            Application::GetWindow()->CursorEnabled(false);
            ImGui::SetWindowFocus("ViewPort: ");
            viewportFocused = true;
        }
        return false;
    });
    dispatcher.dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent& e)
    {
        if (e.GetButton() == MOUSE_BUTTON_RIGHT)
        {
            cameraControlMode = false;
            Application::GetWindow()->CursorEnabled(true);
        }
        return false;
    });
    

    if (!viewportFocused)
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
    MenuBar();
    ImGui::End();

    static bool first = true;
    ApplicationPanel(dt, first);
    AssetManagerPanel(dt, first);
    //LayerPanel(dt, first);
    ViewPortPanel(dt, first);
    //InspectorPanel(dt, first);

    if (first) {
        first = false;
        ImGui::SetWindowFocus("ViewPort: ");
    }
}

void PELayer::DockPanel(std::string name, ImGuiID dock_id)
{
    if (dockPanelQueue.find(name) == dockPanelQueue.end())
    {
        dockPanelQueue.emplace(name, dock_id);
    }
}

void PELayer::MenuBar()
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

    if (ImGui::TreeNode("Camera"))
    {
        if (ImGui::Button("Reset Camera"))
        {
            Application::GetActiveScene()->GetCamera()->fov = 1.0f;
            Application::GetActiveScene()->GetCamera()->position = glm::vec3(0.0f, 0.0f, 5.0f);
        }

        glm::vec3 pos = Application::GetActiveScene()->GetCamera()->position;
        float arrayPos[]{ pos.x, pos.y, pos.z };
        ImGui::DragFloat3("XYZ", arrayPos, 0.5f);
        Application::GetActiveScene()->GetCamera()->position = glm::vec3(arrayPos[0], arrayPos[1], arrayPos[2]);

        float fov = Application::GetActiveScene()->GetCamera()->fov;
        ImGui::DragFloat("FOV", &fov, 0.5f);
        Application::GetActiveScene()->GetCamera()->fov = fov;

        ImGui::TreePop();
    }

    ImGui::End();
}

struct DirectoryNode
{
    std::string FullPath;
    std::string FileName;
    std::vector<DirectoryNode> Children;
    bool IsDirectory;
    bool HasDirectoryChilds;
};

void RecursivelyAddDirectoryNodes(DirectoryNode& parentNode, std::filesystem::directory_iterator directoryIterator)
{
    for (const std::filesystem::directory_entry& entry : directoryIterator)
    {
        DirectoryNode& childNode = parentNode.Children.emplace_back();
        childNode.FullPath = entry.path().u8string();
        childNode.FileName = entry.path().filename().u8string();
        if (childNode.IsDirectory = entry.is_directory(); childNode.IsDirectory)
            RecursivelyAddDirectoryNodes(childNode, std::filesystem::directory_iterator(entry));
    }

    auto moveDirectoriesToFront = [](const DirectoryNode& a, const DirectoryNode& b) { return (a.IsDirectory > b.IsDirectory); };
    std::sort(parentNode.Children.begin(), parentNode.Children.end(), moveDirectoriesToFront);
}

DirectoryNode CreateDirectryNodeTreeFromPath(const std::filesystem::path& rootPath)
{
    DirectoryNode rootNode;
    rootNode.FullPath = rootPath.u8string();
    rootNode.FileName = rootPath.filename().u8string();
    if (rootNode.IsDirectory = std::filesystem::is_directory(rootPath); rootNode.IsDirectory)
        RecursivelyAddDirectoryNodes(rootNode, std::filesystem::directory_iterator(rootPath));

    return rootNode;
}

static std::vector<DirectoryNode> showInPanel;
static bool isAlreadyClicked = false;

void RecursivelyDisplayDirectoryNode(const DirectoryNode& parentNode)
{
    ImGui::PushID(&parentNode);
    if (parentNode.IsDirectory)
    {
        bool hasDirectoryAsChild = false;
        for (auto& node : parentNode.Children)
        {
	        if (node.IsDirectory)
	        {
                hasDirectoryAsChild = true;
                break;
	        }
        }
        if (hasDirectoryAsChild)
        {
            if (ImGui::TreeNodeEx(parentNode.FileName.c_str())) //ImGuiTreeNodeFlags_SpanFullWidth
            {
                for (const DirectoryNode& childNode : parentNode.Children)
                    RecursivelyDisplayDirectoryNode(childNode);
                ImGui::TreePop();
            }
            if (ImGui::IsItemClicked())
            {
                if (!isAlreadyClicked)
                {
                    showInPanel.clear();
                    for (auto& node : parentNode.Children)
                    {
                        if (!node.IsDirectory)
                        {
                            showInPanel.emplace_back(node);
                        }
                    }
                    isAlreadyClicked = true;
                }
            }
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if (ImGui::Button(parentNode.FileName.c_str()))
            {
                showInPanel.clear();
                for (auto& node : parentNode.Children)
                {
                    if (!node.IsDirectory)
                    {
                        showInPanel.emplace_back(node);
                    }
                }
            }
            ImGui::PopStyleColor();
        }
    }
    ImGui::PopID();
}

static DirectoryNode rootNode = CreateDirectryNodeTreeFromPath(L"assets");

void PELayer::AssetManagerPanel(const float dt, bool first)
{
    static std::filesystem::path relPath = "/";
    static const std::filesystem::path absPath = "assets";
    const std::filesystem::path path = absPath.string() + relPath.string();
    std::filesystem::path itemPath = path;

    const float size = 100.0f;

 	const char* name = "Asset Manager: ";
    std::stringstream stream;

    if (first)
        DockPanel(name, GetDockspaceBottom());

    const ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_Resizable;


    ImGui::Begin(name);
    LOG_DEBUG(ImGui::GetContentRegionAvail().x);
    int cols = size > ImGui::GetContentRegionAvail().x ? 1 : (int)ImGui::GetContentRegionAvail().x / size;
    if (cols > 64) cols = 64;

    float con = 0.8f;

    if (ImGui::BeginTable("dirTable", cols))
    {
        ImGui::TableSetupColumn("dirCol");
        ImGui::TableNextRow();

        int i = 0;
        for (const auto item : std::filesystem::directory_iterator(path))
        {
            ImGui::TableSetColumnIndex(i);
            if (item.is_directory())
            {
                if (ImGui::ImageButton((void*)DataPool::GetTexture("folder_icon.png")->GetID(), ImVec2(size * con, size * con), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
                {
                    relPath = relPath.string() + item.path().filename().string() + "/";
                    LOG_DEBUG(relPath);
                }
            }
            else
            {
                if (ImGui::ImageButton((void*)DataPool::GetTexture("file_icon.png")->GetID(), ImVec2(size * con, size * con), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
                {
                    itemPath = path.string() + item.path().filename().string();
                    LOG_DEBUG(itemPath);
                }
            }
            i++;
            if (i >= 9) i = 0;
        }
        ImGui::EndTable();
    }
    
    

    //if (ImGui::BeginTable("table1", 2, flags))
    //{
    //    ImGui::TableSetupColumn("directory", 0, 100);
    //    ImGui::TableNextRow(1, ImGui::GetContentRegionAvail().y);
    //
    //    ImGui::TableSetColumnIndex(0);
    //
    //    //RecursivelyDisplayDirectoryNode(rootNode);
    //
    //
    //
    //	ImGui::TableSetColumnIndex(1);
    //
    //    for (auto& node : showInPanel)
    //    {
    //        Shr<Texture> folderIcon = DataPool::GetTexture("folder_icon.png");
    //        ImGui::Image((void*)folderIcon->GetID(), ImVec2(128.0f, 128.0f), ImVec2(0, 1), ImVec2(1, 0));
    //        ImGui::Text(node.FileName.c_str());
    //    }
    //
    //
    //    ImGui::EndTable();
    //}
    

    ImGui::End();
}

void PELayer::LayerPanel(const float dt, bool first)
{
    const char* name = "Layers: ";
    std::stringstream stream;

    if (first)
        DockPanel(name, GetDockspaceLeft());

    ImGui::Begin(name);

    for (Layer* layer : Application::GetLayerStack())
    {
        if (layer == &Application::GetImGuiLayer()) continue;
        std::vector<Entity*> gameobjects = layer->GetEntitys();
        std::vector<Entity*> entities = Application::GetActiveScene()->GetEntitys();
        std::vector<UIObject*> uiObjects = layer->GetUIObjects();

        if (ImGui::TreeNode(layer->GetName().c_str()))
        {
            if (ImGui::TreeNode("UIObjects: "))
            {
                for (int i = 0; i < uiObjects.size(); i++)
                {
                    if (ImGui::Selectable((uiObjects[i]->GetName() + std::string("##" + std::to_string(i))).c_str(), uiObjects[i] == selectedObject)) {
                        selectedObject = uiObjects[i];
                    }
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Entitys: "))
            {
                int b = 0;
                for (int i = 0; i < gameobjects.size(); i++)
                {
                    if (ImGui::Selectable((gameobjects[i]->GetName() + std::string(" (ObjectID = " + std::to_string(gameobjects[i]->GetCoreID()) + std::string(")")) + std::string("##" + std::to_string(i))).c_str(), gameobjects[i] == selectedObject)) {
                        selectedObject = gameobjects[i];
                    }
                    b = i;
                }
                for (int i = 0; i < entities.size(); i++)
                {
                    if (ImGui::Selectable((entities[i]->GetName() + std::string(" (ObjectID = " + std::to_string(entities[i]->GetCoreID()) + std::string(")")) + std::string("##" + std::to_string(i + b))).c_str(), entities[i] == selectedObject)) {
                        selectedObject = entities[i];
                    }
                }
                ImGui::TreePop();
            }

            ImGui::Text("");
            ImGui::TreePop();
        }
    }

    ImGui::End();
}

void PELayer::InspectorPanel(const float dt, bool first) {
    const char* name = "Inspector: ";
    std::stringstream stream;

    if (first)
        DockPanel(name, GetDockspaceLeftBottom());

    ImGui::Begin(name);
    if (dynamic_cast<Entity*>(selectedObject) != nullptr) {
        dynamic_cast<Entity*>(selectedObject)->Imgui(dt);
    }
    ImGui::End();
}

void PELayer::ViewPortPanel(const float dt, bool first)
{
    const char* name = "ViewPort: ";

    if (first)
        DockPanel(name, GetDockspaceMain());

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin(name);


    //RenderCommand::GetFramebuffer()->Bind();

    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    auto viewportOffset = ImGui::GetWindowPos();
    viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
    viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

    viewportFocused = ImGui::IsWindowFocused();
    viewportHovered = ImGui::IsWindowHovered();

    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    viewportSize = { viewport_panel_size.x, viewport_panel_size.y };

    uint32_t textureID = framebuffer->GetColorID(0);
    ImGui::Image((void*)textureID, ImVec2(viewportSize.x, viewportSize.y), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    ImGui::End();
    ImGui::PopStyleVar();



}
