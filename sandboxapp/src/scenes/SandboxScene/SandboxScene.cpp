#include "_Game.h"
#include "SandboxScene.h"

SandboxScene::SandboxScene()
{
	backcolor = glm::vec4(0.2f);
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
		camera->position.z -= 5 * dt;
	if (Input::IsKeyPressed(KEY_A))
		camera->position.x -= 5 * dt;
	if (Input::IsKeyPressed(KEY_S))
		camera->position.z += 5 * dt;
	if (Input::IsKeyPressed(KEY_D))
		camera->position.x += 5 * dt;
	if (Input::IsKeyPressed(KEY_SPACE))
		camera->position.y += 5 * dt;
	if (Input::IsKeyPressed(KEY_C))
		camera->position.y -= 5 * dt;
}

void SandboxScene::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(SandboxScene::KeyPressed));
	dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(SandboxScene::MouseScrolled));
}

bool SandboxScene::KeyPressed(KeyPressedEvent& event) const
{
	
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
