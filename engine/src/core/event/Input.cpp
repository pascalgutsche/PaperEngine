#include "Engine.h"
#include "Input.h"

#include "generic/Application.h"
#include "imgui/ImGuiLayer.h"

#include "GLFW/glfw3.h"

namespace Paper
{
	std::unordered_map<Key, KeyState> Input::keyStates;

	bool Input::IsKeyPressed(Key code)
	{
		return keyStates.contains(code) && keyStates[code] == KeyState::Pressed;
	}

	bool Input::IsKeyHeld(Key code)
	{
		return !IsKeyPressed(code) && IsKeyDown(code);
	}

	bool Input::IsKeyDown(Key code)
	{
		auto state = glfwGetKey((GLFWwindow*)Application::GetWindow()->GetNativeWindow(), (int)code);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsKeyReleased(Key code)
	{
		return keyStates.contains(code) && keyStates[code] == KeyState::Released;
	}

	bool Input::IsMouseButtonPressed(MouseButton button)
	{
		auto state = glfwGetMouseButton((GLFWwindow*)Application::GetWindow()->GetNativeWindow(), (int)button);
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousPos()
	{
		double posX, posY;
		glfwGetCursorPos((GLFWwindow*)Application::GetWindow()->GetNativeWindow(), &posX, &posY);
		return glm::vec2(posX, posY);
	}

	void Input::UpdateKeyState(Key key, KeyState keyState)
	{
		keyStates[key] = keyState;
	}

	void Input::Update()
	{
		keyStates.clear();
	}
}
