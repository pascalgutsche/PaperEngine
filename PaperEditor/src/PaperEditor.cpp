#include "Editor.h"

#define INCLUDE_MAIN
#include "EntryPoint.h"
#include "editor/PELayer.h"

#include "scene/EditorScene/EditorScene.h"
#include "project/ProjectManager.h"

#include "editor/PELayer.h"

class PaperEditor : public Application {
public:
	PaperEditor()
		: Application(WindowProps("PaperEditor"))
	{
		
	}
	
	~PaperEditor() override {
		
	}

	void Init() override {
		ProjectManager::Init();
		//Application::ChangeScene(new EditorScene);

		AddLayer(new PELayer());

		EditorScene* scene = new EditorScene();
		scene->CreateEntity("lol");

		LOG_DEBUG(scene->registry);
	}

};

Application* engine::CreateApplication() {
	return new PaperEditor();
}