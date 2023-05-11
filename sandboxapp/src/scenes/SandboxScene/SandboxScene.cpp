#include "_Game.h"
#include "SandboxScene.h"

SandboxScene::SandboxScene()
{
	backcolor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	uiLayer = new UILayer();
}

SandboxScene::~SandboxScene()
{

}

void SandboxScene::OnStart()
{
	AddOverlay(uiLayer);
}

void SandboxScene::OnStop()
{

}

void SandboxScene::OnUpdate()
{
	const float dt = Application::GetDT();
	if (Input::IsKeyPressed(KEY_W))
	{
		camera->position.x += 5 * dt * camera->GetFront().x;
		camera->position.z += 5 * dt * camera->GetFront().z;
	}
	if (Input::IsKeyPressed(KEY_A))
	{
		camera->position.x += 5 * dt * camera->GetFront().z;
		camera->position.z -= 5 * dt * camera->GetFront().x;
		
	}
	if (Input::IsKeyPressed(KEY_S))
	{
		camera->position.x -= 5 * dt * camera->GetFront().x;
		camera->position.z -= 5 * dt * camera->GetFront().z;
		
	}
	if (Input::IsKeyPressed(KEY_D))
	{
		camera->position.x -= 5 * dt * camera->GetFront().z;
		camera->position.z += 5 * dt * camera->GetFront().x;
		
	}
	if (Input::IsKeyPressed(KEY_SPACE))
		camera->position.y += 5 * dt;
	if (Input::IsKeyPressed(KEY_C))
		camera->position.y -= 5 * dt;
}

void SandboxScene::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(SandboxScene::KeyPressed));
	dispatcher.dispatch<KeyReleasedEvent>(BIND_EVENT_FN(SandboxScene::KeyReleased));
	dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(SandboxScene::MouseScrolled));
	dispatcher.dispatch<WindowFocusEvent>(BIND_EVENT_FN(SandboxScene::WindowFocus));
	dispatcher.dispatch<WindowLostFocusEvent>(BIND_EVENT_FN(SandboxScene::WindowLostFocus));
}

bool SandboxScene::KeyPressed(KeyPressedEvent& event) const
{
	if (event.getKeyCode() == KEY_LEFT_ALT)
	{
		Application::GetWindow()->CursorEnabled(!Application::GetWindow()->IsCursorEnabled());
	}
	if (event.getKeyCode() == KEY_ESCAPE)
	{
		Application::GetWindow()->CursorEnabled(true);
	}
	return false;
}

bool SandboxScene::KeyReleased(KeyReleasedEvent& event) const
{
	//if (event.getKeyCode() == KEY_LEFT_ALT)
	//{
	//	Application::GetWindow()->CursorEnabled(false);
	//}
	return false;
}

bool SandboxScene::MouseScrolled(MouseScrolledEvent& event) const
{
	if (event.GetYOffset() < 0 && camera->fov > -120.0f)
	{
		camera->fov += event.GetYOffset() / 10;
		return true;
	}
	if (event.GetYOffset() > 0 && camera->fov < 120.0f)
	{
		camera->fov += event.GetYOffset() / 10;
		return true;
	}
	return false;
}

bool SandboxScene::WindowFocus(WindowFocusEvent& event) const
{
	Application::GetWindow()->CursorEnabled(true);
	return true;
}

bool SandboxScene::WindowLostFocus(WindowLostFocusEvent& event) const
{
	Application::GetWindow()->CursorEnabled(true);
	return true;
}
