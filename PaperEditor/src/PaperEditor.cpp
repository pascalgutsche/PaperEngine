#include "Editor.h"

#define INCLUDE_MAIN
#include "EntryPoint.h"
#include "editor/PaperLayer.h"

#include "project/ProjectManager.h"

#include <ImGuizmo/ImGuizmo.h>

class PaperEditor : public Application {
public:
	PaperEditor()
		: Application(WindowProps("PaperEditor"))
	{

	}
	
	~PaperEditor() override
	{
		
	}

	void Init() override {
		ProjectManager::Init();
		
		AddLayer(new PaperLayer());
	}

	void BeforePollEvents() override
	{
		is_using_imguizmo = ImGuizmo::IsUsing();
	}

	bool IsUsingImGuizmo()
	{
		return is_using_imguizmo;
	}

private:
	bool is_using_imguizmo = false;

};

Application* Paper::CreateApplication() {
	return new PaperEditor();
}