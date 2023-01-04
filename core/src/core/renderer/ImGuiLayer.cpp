#include "_Core.h"

#include "renderer/ImGuiLayer.h"
#include "generic/Window.h"

namespace core {

    void ImGuiLayer::init() {
        IMGUI_CHECKVERSION();
        //create context to OpenGL
        ImGui::CreateContext();
        // enable dark mode
        ImGui::StyleColorsDark();
        //ImGui Config
        ImGuiIO io = ImGui::GetIO();
        io.IniFilename = "config/imgui/imgui.ini";
        io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags = ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.BackendPlatformName = "imgui_impl_glfw";

        //fonts
        ImFontAtlas* fontAtlas = io.Fonts;
        ImFontConfig fontConfig = ImFontConfig();

        //set default range (uft-8)
        fontConfig.GlyphRanges = fontAtlas->GetGlyphRangesDefault();

        fontAtlas->AddFontFromFileTTF("assets/fonts/mononoki.ttf", 24, &fontConfig);

        //any new fonts were added to the font pool
        fontConfig.MergeMode = true;

        //init backend
        ImGui_ImplGlfw_InitForOpenGL(Window::getGLFWwindow(), true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void ImGuiLayer::destroy() {
        ImGui::DestroyContext();
        delete this->p_open;
    }

    void ImGuiLayer::startFrame(float deltaTime) {

        int width = 0, height = 0;
        int fbWidth = 0, fbHeight = 0;
        double mousePosX = 0, mousePosY = 0;

        glfwGetWindowSize(Window::getGLFWwindow(), &width, &height);
        glfwGetFramebufferSize(Window::getGLFWwindow(), &fbWidth, &fbHeight);
        glfwGetCursorPos(Window::getGLFWwindow(), &mousePosX, &mousePosY);

        ImGuiIO io = ImGui::GetIO();
        io.DisplaySize = ImVec2(Window::width, Window::height);
        io.MousePos = ImVec2((float)mousePosX, (float)mousePosY);
        io.DeltaTime = deltaTime;
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

    }

    void ImGuiLayer::endFrame() {
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(Window::getGLFWwindow(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::update(float deltaTime, Scene* currentScene) {
        startFrame(deltaTime);

        //configureDocking();
        currentScene->sceneImgui(deltaTime);
        //ImGui::End();

        endFrame();
    }

    void ImGuiLayer::configureDocking() {
        //Dock space
        int imguiFlags = ImGuiWindowFlags_MenuBar;// | ImGuiWindowFlags_NoDocking;

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(Window::width, Window::height));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        imguiFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        this->p_open = new bool(true);
        ImGui::Begin("Docking", p_open, imguiFlags);
        ImGui::PopStyleVar(2);

        ImGui::DockSpace(ImGui::GetID("Dockspace"));
    }

}