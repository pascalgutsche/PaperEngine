#include "Editor.h"

#define INCLUDE_MAIN
#include "EntryPoint.h"
#include "editor/PaperLayer.h"

#include <ImGuizmo/ImGuizmo.h>

class PaperEditor : public Application {
public:
	PaperEditor()
		: Application(WindowProps("PaperEditor", 2560, 1200))
	{

	}
	
	~PaperEditor() override
	{
		
	}

	void Init() override {
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