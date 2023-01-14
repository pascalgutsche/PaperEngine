#include "_Core.h"
#include "utility.h"

#include "imgui/ImGuiLayer.h"
#include "generic/Application.h"



namespace core {

    static void darkmode(ImGuiIO& io, ImGuiStyle& style)
    {
        // enable dark mode
        ImGui::StyleColorsDark();

        auto& colors = style.Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
        }
    }



    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
        detach();
    }

    void ImGuiLayer::attach()
    {
        IMGUI_CHECKVERSION();
        //create context to OpenGL
        ImGui::CreateContext();
        ImPlot::CreateContext();

        //ImGui Config
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();

        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        darkmode(io, style);

        //fonts
        ImFontAtlas* fontAtlas = io.Fonts;
        ImFontConfig& fontConfig = ImFontConfig();

        //set default range (uft-8)
        fontConfig.GlyphRanges = fontAtlas->GetGlyphRangesDefault();

        fontAtlas->AddFontFromFileTTF("assets/fonts/mononoki.ttf", 24, &fontConfig);

        //any new fonts were added to the font pool
        fontConfig.MergeMode = true;

        //init backend
        ImGui_ImplGlfw_InitForOpenGL(Application::getWindow()->getNativeWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::detach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::begin(const float dt)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(Application::getWindow()->getWidth(), Application::getWindow()->getHeight());
        io.DeltaTime = dt;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::end()
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(Application::getWindow()->getWidth(), Application::getWindow()->getHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* context_current = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(context_current);
        }
    }


    void ImGuiLayer::DockPanel(std::string name, ImGuiID dock_id)
    {
        if (dock_panel_queue.find(name) == dock_panel_queue.end())
        {
            dock_panel_queue.emplace(name, dock_id);
        }
    }

    static bool p_open = true;
    void ImGuiLayer::imgui(const float dt)
    {

        ImGuiDockNodeFlags dockflags = ImGuiDockNodeFlags_PassthruCentralNode;//ImGuiDockNodeFlags_None; 

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
        window_flags |= ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;// | ImGuiWindowFlags_MenuBar;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        ImGuiViewport& viewport = *ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport.Pos);
        ImGui::SetNextWindowSize(viewport.Size);
        ImGui::SetNextWindowViewport(viewport.ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("docking", &p_open, window_flags);
        ImGui::PopStyleVar(3);

        dockspace_id = ImGui::GetID("dockspace");

        static bool initialized = false;
        if (!initialized)
        {
            initialized = true;
            ImGui::DockBuilderRemoveNode(dockspace_id);
            ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspace_id, ImVec2(Application::getWindow()->getWidth() + 500, Application::getWindow()->getHeight() + 500));

            dock_id_main = dockspace_id;
            dock_id_right = ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Right, 0.15f, nullptr, &dock_id_main);
            dock_id_left = ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Left, 0.2f, nullptr, &dock_id_main);
            dock_id_top = ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Up, 0.2f, nullptr, &dock_id_main);
            dock_id_down = ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Down, 0.25f, nullptr, &dock_id_main);
            dock_id_right2 = ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Left, 0.2f, nullptr, &dock_id_right);


            ImGui::DockBuilderFinish(dockspace_id);
        }
        for (auto [key, val] : dock_panel_queue)
        {
            ImGui::DockBuilderDockWindow(key.c_str(), val);
        }
        dock_panel_queue.clear();

        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockflags);
        ImGui::End();

        ApplicationPanel(dt);
        ScenePanel(dt);
        LayerPanel(dt);
        ViewPortPanel(dt);

    }

	void ImGuiLayer::ApplicationPanel(const float dt)
    {
        const char* name = "Application: ";
        std::stringstream stream;
        Application::IMGUI().DockPanel(name, Application::IMGUI().getDockspaceRIGHT());

        ImGui::Begin(name);

        static float time = 0;
        static float history = 3;
        static int flags = ImPlotAxisFlags_NoTickLabels;
        static ScrollingBuffer sbuff_dt(50000), sbuff_fps(50000);

        time += dt;
        sbuff_dt.AddPoint(time, 1000 * dt);
        sbuff_fps.AddPoint(time, (1 / dt));
        ImGui::SliderFloat("Time", &history, 1, 50, "%.1f");

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
        if (ImPlot::BeginPlot("##frames_per_second", ImVec2(-1, 100))) {
            ImPlot::SetupAxes(NULL, NULL, flags, flags);
            ImPlot::SetupAxisLimits(ImAxis_X1, time - history, time, ImGuiCond_Always);
            ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 90);
            ImPlot::SetNextFillStyle(ImVec4(1.0f, 0.0f, 0.0f, -1.0f), 0.5f);
            ImPlot::PlotLine("##frames", &sbuff_fps.Data[0].x, &sbuff_fps.Data[0].y, sbuff_fps.Data.size(), 0, sbuff_fps.Offset, 2 * sizeof(float));
            ImPlot::EndPlot();
        }

        stream << "Frames rendered: " << Application::GetFramesRendered();
        ImGui::BulletText(stream.str().c_str()); stream.str("");

        ImGui::Text("Render Stats:");
        ImGui::BulletText("draw calls: N/A");
        ImGui::BulletText("vertices: N/A");
        ImGui::BulletText("draw calls: N/A");
        ImGui::BulletText("draw calls: N/A");

        static bool vsync = true;
        ImGui::Checkbox("V-Sync", &vsync);
        Application::getWindow()->setVSync(vsync);

        ImGui::End();
    }

    void ImGuiLayer::ScenePanel(const float dt)
    {
        const char* name = "Scenes: ";
        std::stringstream stream;
        Application::IMGUI().DockPanel(name, Application::IMGUI().getDockspaceBOTTOM());

        ImGui::Begin(name);
        ImGui::End();
    }

    void ImGuiLayer::LayerPanel(const float dt)
    {
        const char* name = "Layers: ";
        std::stringstream stream;
        Application::IMGUI().DockPanel(name, Application::IMGUI().getDockspaceLEFT());

        ImGui::Begin(name);
        ImGui::End();
    }

    void ImGuiLayer::ViewPortPanel(const float dt)
    {
        const char* name = "ViewPort: ";
        std::stringstream stream;
        Application::IMGUI().DockPanel(name, Application::IMGUI().getDockspaceMAIN());

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin(name);

        ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
        if (viewport_size != *(glm::vec2*)&viewport_panel_size)
        {
            viewport_size = { viewport_panel_size.x, viewport_panel_size.y };
            Application::getCurrentScene()->GetRenderer().GetFrameBuffer().Resize(viewport_size.x, viewport_size.y);
        }
        uint32_t textureID = Application::getCurrentScene()->GetRenderer().GetFrameBuffer().GetColorID();

        ImGui::Image((void*)textureID, ImVec2(viewport_size.x, viewport_size.y), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        ImGui::End();
        ImGui::PopStyleVar();
    }

}