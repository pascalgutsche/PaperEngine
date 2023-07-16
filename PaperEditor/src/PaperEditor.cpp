#include "Editor.h"

#define INCLUDE_MAIN
#include "EntryPoint.h"
#include "editor/PELayer.h"

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
		//Application::ChangeScene(new EditorScene);
		
		AddLayer(new PELayer());

		
		//Shr<Scene> scene = MakeShr<Scene>();
		//Paper::UUID uuid = scene->CreateEntity("lol").GetUUID();
		//
		//scene->GetEntity(uuid).AddComponent<SpriteComponent>();
		//scene->GetEntity(uuid).AddComponent<CircleComponent>();
		//scene->GetEntity(uuid).AddComponent<LineComponent>();
		//scene->GetEntity(uuid).AddComponent<TextComponent>();
		//scene->GetEntity(uuid).AddTag({ "A", "B", "C", "AA", "aB", "Ac" });
		//
		//const YAMLSerializer serializer;
		//serializer.SceneSerialize("bunker.yaml", scene);
		//
		//LOG_CORE_WARN("DESERIALIZE");
		//LOG_CORE_DEBUG(uuid.toString());
		//LOG_CORE_WARN(serializer.SceneDeserialize("bunker.yaml")->GetEntity(uuid).GetName());

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