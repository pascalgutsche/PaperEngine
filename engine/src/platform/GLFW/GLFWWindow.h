#pragma once
#include "Engine.h"
#include "utility.h"

#include "generic/Window.h"
#include "renderer/Context.h"

#include <GLFW/glfw3.h>

namespace Paper
{
	class GLFWWindow : public Window
	{
    public:

        GLFWWindow(const WindowProps& window_props);
        ~GLFWWindow() override;

        void PollEvents() override;
        void SwapBuffers() override;

        unsigned int GetWidth() const override { return windowData.width; }
        unsigned int GetHeight() const override { return windowData.height; }

        glm::ivec2 GetPosition() const override;

        float GetTime() const override;

        void SetEventCallback(const EventCallbackFunction& callback_function) override { windowData.callback = callback_function; }
        void SetTitle(const std::string& title) override;
        void SetVSync(bool enabled) override;
        bool IsVSync() const override { return windowData.vsync; }

        void CursorEnabled(bool enabled) override;
        bool IsCursorEnabled() const override;

        void* GetNativeWindow() const override;

    private:
        GLFWwindow* glfwWindow;
        Shr<Context> context;

        bool cursorEnabled = true;

        bool initialized = false;

        struct WindowData
        {
            std::string title;
            unsigned int width, height;
            bool vsync;
            EventCallbackFunction callback;
        };

        WindowData windowData;


        void Init(const WindowProps& window_props);
        void Quit() const;
	};
	
}

