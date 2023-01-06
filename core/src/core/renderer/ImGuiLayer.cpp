#include "_Core.h"
#include "utility.h"

#include "renderer/ImGuiLayer.h"
#include "generic/Window.h"

#include <IMGUI/imgui.h>
#include "OpenGL/ImGui_OpenGL3.h"

namespace core {

    void ImGuiLayer::init() {
        IMGUI_CHECKVERSION();
        //create context to OpenGL
        ImGui::CreateContext();
        // enable dark mode
        ImGui::StyleColorsDark();
        //ImGui Config
        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        io.IniFilename = "config/imgui/imgui.ini";
        io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;
        //io.ConfigFlags = ImGuiConfigFlags_DockingEnable;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.BackendPlatformName = "imgui_impl_glfw";

        //fonts
        ImFontAtlas* fontAtlas = io.Fonts;
        ImFontConfig& fontConfig = ImFontConfig();

        //set default range (uft-8)
        fontConfig.GlyphRanges = fontAtlas->GetGlyphRangesDefault();

        fontAtlas->AddFontFromFileTTF("assets/fonts/mononoki.ttf", 24, &fontConfig);

        //any new fonts were added to the font pool
        fontConfig.MergeMode = true;

        //init backend
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::destroy() {
        
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
        ImGui::NewFrame();

    }


    void ImGuiLayer::update(float deltaTime, Scene* currentScene) {
        startFrame(deltaTime);

        currentScene->sceneImgui(deltaTime);
        ImGui::ShowDemoWindow();

        endFrame();
    }

    void ImGuiLayer::endFrame() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}