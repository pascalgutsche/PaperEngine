#include "Editor.h"

#define INCLUDE_MAIN
#include "EntryPoint.h"

#include "scene/EditorScene/EditorScene.h"

class PaperEditor : public Application {
public:
	PaperEditor()
		: Application(WindowProps("PaperEditor"))
	{
		
	}
	
	~PaperEditor() override {
		
	}

	void Init() override {
		Application::ChangeScene(new EditorScene); 
	}

};

Application* engine::CreateApplication() {
	return new PaperEditor();
}