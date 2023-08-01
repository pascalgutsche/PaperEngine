#include "Engine.h"
#include "Input.h"

#include "generic/Application.h"
#include "imgui/ImGuiLayer.h"

#include "GLFW/glfw3.h"

namespace Paper
{
	bool Input::IsKeyPressed(Key code)
	{
		auto state = glfwGetKey((GLFWwindow*)Application::GetWindow()->GetNativeWindow(), (int)code);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
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
}
