#include "Editor.h"

#define INCLUDE_MAIN
#include "EntryPoint.h"

#include "scene/EditorScene/EditorScene.h"
#include "project/ProjectManager.h"

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
		Application::ChangeScene(new EditorScene);
	}

};

Application* engine::CreateApplication() {
	return new PaperEditor();
}