#include "Editor.h"
#include "EditorScene.h"

#include "layers/PELayer.h"
#include "layers/UILayer.h"

EditorScene::EditorScene()
{
	backcolor = glm::vec3(0.2f, 0.2f, 0.2f);
}

EditorScene::~EditorScene()
{
}

void EditorScene::OnStart()
{
	AddLayer(new UILayer());
	AddLayer(new PELayer());
}

void EditorScene::OnStop()
{
}

void EditorScene::OnUpdate()
{
}

void EditorScene::OnEvent(Event& e)
{
}
