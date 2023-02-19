#include "_Core.h"
#include "Input.h"

#include "generic/Application.h"

#include "GLFW/glfw3.h"

namespace core
{
	bool Input::IsKeyPressed(int key_code)
	{
		auto state = glfwGetKey((GLFWwindow*)Application::GetWindow()->GetNativeWindow(), key_code);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto state = glfwGetMouseButton((GLFWwindow*)Application::GetWindow()->GetNativeWindow(), button);
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousPos()
	{
		double posX, posY;
		glfwGetCursorPos((GLFWwindow*)Application::GetWindow()->GetNativeWindow(), &posX, &posY);
		return glm::vec2(posX, posY);
	}
}
