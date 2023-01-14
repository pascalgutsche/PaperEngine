#include "_Core.h"
#include "Input.h"

#include "generic/Application.h"

#include "GLFW/glfw3.h"

namespace core
{
	bool Input::IsKeyPressed(int key_code)
	{
		auto state = glfwGetKey(Application::getWindow()->getNativeWindow(), key_code);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto state = glfwGetMouseButton(Application::getWindow()->getNativeWindow(), button);
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousPos()
	{
		double posX, posY;
		glfwGetCursorPos(Application::getWindow()->getNativeWindow(), &posX, &posY);
		return glm::vec2(posX, posY);
	}
}
