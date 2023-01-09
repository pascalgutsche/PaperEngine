#include "_Core.h"
#include "utility.h"

#include "layer/ImGuiLayer.h"
#include "generic/Application.h"
#include "event/KeyCodes.h"

#include <imgui.h>
#include "OpenGL/ImGui_OpenGL3.h"

#include "glad/glad.h"

namespace core {

    static void darkmode()
    {
        auto& colors = ImGui::GetStyle().Colors;
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
        //colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        //colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        //colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        //colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        //colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    }

    

    ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
        attach();
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::attach()
	{
        IMGUI_CHECKVERSION();
        //create context to OpenGL
        ImGui::CreateContext();
        // enable dark mode
        ImGui::StyleColorsDark();
        //ImGui Config
        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        io.KeyMap[ImGuiKey_Tab] = KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = KEY_HOME;
        io.KeyMap[ImGuiKey_End] = KEY_END;
        io.KeyMap[ImGuiKey_Insert] = KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = KEY_ESCAPE;
        io.KeyMap[ImGuiKey_A] = KEY_A;
        io.KeyMap[ImGuiKey_C] = KEY_C;
        io.KeyMap[ImGuiKey_V] = KEY_V;
        io.KeyMap[ImGuiKey_X] = KEY_X;
        io.KeyMap[ImGuiKey_Y] = KEY_Y;
        io.KeyMap[ImGuiKey_Z] = KEY_Z;

        //io.IniFilename = "config/imgui/imgui.ini";
        //io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;
        ////io.ConfigFlags = ImGuiConfigFlags_DockingEnable;
        ////io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        //io.BackendPlatformName = "imgui_impl_glfw";
        //
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

	void ImGuiLayer::detach()
	{

	}

	void ImGuiLayer::update(const float dt)
	{
        begin(dt);

        ImGui::ShowDemoWindow();

        end();
	}


	void ImGuiLayer::begin(const float dt)
	{
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(Application::getWindow()->getWidth(), Application::getWindow()->getHeight());
        //io.MousePos = ImVec2((float)mousePosX, (float)mousePosY);
        io.DeltaTime = dt;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
	}

	void ImGuiLayer::end()
	{
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::event(Event& event)
	{
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiLayer::onMouseButtonPressed));
        dispatcher.dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::onMouseButtonReleased));
        dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ImGuiLayer::onMouseScrolled));
        dispatcher.dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImGuiLayer::onMouseMoved));
        dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::onKeyPressed));
        dispatcher.dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::onKeyReleased));
        dispatcher.dispatch<KeyTypedEvent>(BIND_EVENT_FN(ImGuiLayer::onKeyTyped));
        dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImGuiLayer::onWindowResize));
	}

    bool ImGuiLayer::onMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.getButton()] = true;

        return false;
    }

    bool ImGuiLayer::onMouseButtonReleased(MouseButtonReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.getButton()] = false;

        return false;
    }

    bool ImGuiLayer::onMouseScrolled(MouseScrolledEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel += e.getYOffset();
        io.MouseWheelH += e.getYOffset();

        return false;
    }

    bool ImGuiLayer::onMouseMoved(MouseMovedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.getX(), e.getY());

        return false;
    }

    bool ImGuiLayer::onKeyPressed(KeyPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.getKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[KEY_LEFT_CONTROL] || io.KeysDown[KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[KEY_LEFT_SHIFT] || io.KeysDown[KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[KEY_LEFT_ALT] || io.KeysDown[KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[KEY_LEFT_WINDOWS] || io.KeysDown[KEY_RIGHT_WINDOWS];

        return false;
    }

    bool ImGuiLayer::onKeyReleased(KeyReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.getKeyCode()] = false;

        return false;
    }

    bool ImGuiLayer::onKeyTyped(KeyTypedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        int key_code = e.getKeyCode();
		if (key_code > 0 && key_code < 0x100000)
		{
            io.AddInputCharacter((unsigned short)key_code);
		}

        return false;
    }

    bool ImGuiLayer::onWindowResize(WindowResizeEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(e.getWidth(), e.getHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, e.getWidth(), e.getHeight());

        return false;
    }




}