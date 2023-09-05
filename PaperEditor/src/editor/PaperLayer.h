#pragma once
#include "Editor.h"

#include "PanelManager.h"
#include "ViewPort.h"
#include "project/Project.h"
#include "scene/Entity.h"

enum CameraModes
{
	Single, Split, Triple, Quadro
};

inline std::string CameraModesToString(CameraModes mode)
{
	switch (mode) {
		case Single: return "Single";
		case Split: return "Split";
		case Triple: return "Triple";
		case Quadro: return "Quadro";
		default: return "";
	}
}

enum class SceneState
{
	Edit,
	Play,
	Simulate
};

struct PopupData
{
	std::string title;
	std::function<void()> renderFunc;

	std::string message;

	uint32_t width = 500;
	uint32_t height = 0;

	bool shouldOpen = true;
	bool isOpen = false;

	bool autoresize = false;
	bool okBtn = false;
};

inline std::vector<PopupData> popups;

static void ShowMessagePopup(const std::string& title, const std::string& message)
{
	PopupData data;
	data.title = fmt::format("{0}##popup_message_{1}", title, popups.size());
	data.message = message;
	data.shouldOpen = true;
	data.okBtn = true;
	popups.push_back(data);
}

inline void ShowPopup(const std::string& title, std::function<void()> renderFunc)
{
	PopupData data;
	data.title = fmt::format("{0}##popup_{1}", title, popups.size());
	data.renderFunc = renderFunc;
	data.shouldOpen = true;
	data.autoresize = true;
	popups.push_back(data);
}

inline void RenderPopups()
{
	for (PopupData& data : popups)
	{
		if (data.shouldOpen && !ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId))
		{
			data.isOpen = true;
			data.shouldOpen = false;
			ImGui::OpenPopup(data.title.c_str());
		}

		if (!data.isOpen) continue;

		if (!ImGui::IsPopupOpen(data.title.c_str()))
		{
			data.isOpen = false;
			continue;
		}

		uint32_t flags = ImGuiWindowFlags_NoSavedSettings;
		if (data.autoresize) 
			flags |= ImGuiWindowFlags_AlwaysAutoResize;
		else
			ImGui::SetNextWindowSize(ImVec2(data.width, data.height));

		if (ImGui::BeginPopupModal(data.title.c_str(), &data.isOpen, flags))
		{
			if (data.renderFunc)
				data.renderFunc();
			else
			{
				ImGui::TextWrapped(data.message.c_str());
			}

			if (data.okBtn)
			{
				ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize("Okay").x + 5));
				if (ImGui::Button("Okay"))
					ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	for (int i = 0; i < popups.size(); i++)
	{
		if (!popups[i].isOpen && !popups[i].shouldOpen)
		{
			popups.erase(popups.begin() + i);
		}
	}

}

class PaperLayer : public Layer
{
	friend struct ViewPort;
public:
	PaperLayer();
	~PaperLayer() override;

	void OnAttach() override;
	void OnDetach() override;
	void Update(const float dt) override;
	void OnEvent(Event& event) override;
	void EntityDragging();
	void Imgui(const float dt) override;

	
	void CreateProject(const std::filesystem::path& projPath, const std::string& projName);
	void OpenProject(const std::filesystem::path& projPath);
	void OpenProject(const Shr<Project>& project);
	void EmptyProject();
	void SaveProject() const;
	void CloseProject();

	void NewScene(const std::string& sceneName = "NewScene");
	void OpenScene(const Shr<Scene>& scene, bool saveOlsScene = false);
	void SaveScene() const;
	void SaveSceneAs(const std::filesystem::path& filePath) const;

	void GenerateProjectSolution(const std::filesystem::path& projPath) const;

	Entity drag_entity;
	Entity hovered_entity;
	//Entity active_entity;

	int GetGuizmoType() const { return gizmo_type; }

	bool AnyCameraActive() const;
	const ViewPort* GetViewportFocused() const;

	void SwitchScene(const Shr<Scene>& scene);

	void ShowNewProjectPopup();
	void ShowNewScenePopup();

public:

	Shr<Scene> editorScene = nullptr;

	//---- to refactor
	void MousePicking();
	void ViewPortPanel();
	void CameraMode();
	void DockCameraPanel(CameraModes mode, ImGuiID main_id, const ImVec2& dockspace_size);
	void EnableCamera(CameraModes mode);
	//----

	PanelManager panelManager;

	void UI_MenuBar();
	void UI_Toolbar();

	void ChangeWindowTitle() const;


	


	ViewPort* viewPortCurrentlySimulating = nullptr;
	ViewPort* lastFocusedViewPort = nullptr;

	int gizmo_type = -1;

	CameraModes camera_mode = Single;
	bool camera_mode_changed = true;
	std::vector<ViewPort> viewports;

	SceneState sceneState = SceneState::Edit;

	void OnScenePlay();
	void OnSceneSimulate();
	void OnSceneStop();
};

