#include "Editor.h"
#include "DockManager.h"

namespace PaperED
{
	void DockManager::CreateNodes()
	{
		dockspace_id = ImGui::GetID("dockspace");
		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, ImVec2(Application::GetWindow()->GetWidth() + 500, Application::GetWindow()->GetHeight() + 500));

		dockIDs[(size_t)DockLoc::Main] = dockspace_id;
		dockIDs[(size_t)DockLoc::Left] = ImGui::DockBuilderSplitNode(dockIDs[(size_t)DockLoc::Main], ImGuiDir_Left, 0.2f, nullptr, &dockIDs[(size_t)DockLoc::Main]);
		dockIDs[(size_t)DockLoc::Right] = ImGui::DockBuilderSplitNode(dockIDs[(size_t)DockLoc::Main], ImGuiDir_Right, 0.2f, nullptr, &dockIDs[(size_t)DockLoc::Main]);
		dockIDs[(size_t)DockLoc::Bottom] = ImGui::DockBuilderSplitNode(dockIDs[(size_t)DockLoc::Main], ImGuiDir_Down, 0.2f, nullptr, &dockIDs[(size_t)DockLoc::Main]);
		dockIDs[(size_t)DockLoc::Top] = ImGui::DockBuilderSplitNode(dockIDs[(size_t)DockLoc::Main], ImGuiDir_Up, 0.04f, nullptr, &dockIDs[(size_t)DockLoc::Main]);
		dockIDs[(size_t)DockLoc::LeftBottom] = ImGui::DockBuilderSplitNode(dockIDs[(size_t)DockLoc::Left], ImGuiDir_Down, 0.5f, nullptr, &dockIDs[(size_t)DockLoc::Left]);
		dockIDs[(size_t)DockLoc::RightBottom] = ImGui::DockBuilderSplitNode(dockIDs[(size_t)DockLoc::Right], ImGuiDir_Down, 0.5f, nullptr, &dockIDs[(size_t)DockLoc::Right]);

		// Disable tab bar for custom toolbar
		ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockIDs[(size_t)DockLoc::Main]);
		node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

		node = ImGui::DockBuilderGetNode(dockIDs[(size_t)DockLoc::Top]);
		node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

		ImGui::DockBuilderFinish(dockspace_id);
	}

	void DockManager::Update()
	{
		for (auto [key, val] : dockPanelQueue)
		{
			ImGui::DockBuilderDockWindow(key.c_str(), val);
		}
		dockPanelQueue.clear();
	}

	void DockManager::DockPanel(const std::string& panelName, DockLoc dockLocation)
	{
		if (dockLocation == DockLoc::NONE) return;
		if (dockPanelQueue.find(panelName) == dockPanelQueue.end())
		{
			dockPanelQueue.emplace(panelName, dockIDs[(size_t)dockLocation]);
		}
	}
}