#include "Editor.h"
#include "EditorScene.h"


#include "layers/UILayer.h"

EditorScene::EditorScene()
{

	//Entity* testObject = new Entity("test");
	//testObject->AddComponent(new CubeRenderer(glm::vec4(1.0f, 0.5f, 0.31f, 1.0f)));
	//AddEntityToScene(testObject);
}

EditorScene::~EditorScene()
{
}

void EditorScene::OnStart()
{
	//AddLayer(uiLayer);
	//AddLayer(peLayer);
}

void EditorScene::OnStop()
{
}

void EditorScene::OnUpdate()
{
	ImGui::ShowDemoWindow();
	
}

void EditorScene::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(EditorScene::MouseScrolled));
	dispatcher.dispatch<WindowFocusEvent>(BIND_EVENT_FN(EditorScene::WindowFocus));
	dispatcher.dispatch<WindowLostFocusEvent>(BIND_EVENT_FN(EditorScene::WindowLostFocus));
}



bool EditorScene::MouseScrolled(MouseScrolledEvent& event) const
{
	//if (!peLayer->IsCameraControlModeActive()) return false;
	//if (event.GetYOffset() < 0 && camera->fov > 0.01f)
	//{
	//	camera->fov += event.GetYOffset() / 5;
	//	return true;
	//}
	//if (camera->fov < 0.01f) camera->fov = 0.01f;
	//if (event.GetYOffset() > 0 && camera->fov < 120.0f)
	//{
	//	camera->fov += event.GetYOffset() / 5;
	//	return true;
	//}
	return false;
}

bool EditorScene::WindowFocus(WindowFocusEvent& event) const
{
	Application::GetWindow()->CursorEnabled(true);
	return true;
}

bool EditorScene::WindowLostFocus(WindowLostFocusEvent& event) const
{
	Application::GetWindow()->CursorEnabled(true);
	return true;
}
