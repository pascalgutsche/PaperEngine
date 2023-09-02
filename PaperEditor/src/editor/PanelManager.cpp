#include "Editor.h"
#include "PanelManager.h"

#include "DockManager.h"
#include "project/Project.h"

void PanelManager::OnImGuiRender()
{
	for (size_t i = 0; i < (size_t)PanelOpenSetting::_COUNT; i++)
	{
		for (PanelData& panelData : panels[i] | std::views::values)
		{
			if (panelData.isOpen)
			{
				if (panelData.firstRender)
				{
					DockManager::DockPanel(panelData.displayName, panelData.initialDockLocation);
					panelData.firstRender = false;
				}
				panelData.panel->OnImGuiRender(panelData.isOpen);

			}
		}
	}
}

void PanelManager::OnEvent(Event& e)
{
	for (size_t i = 0; i < (size_t)PanelOpenSetting::_COUNT; i++)
	{
		for (const PanelData& panelData : panels[i] | std::views::values)
		{
			if (panelData.isOpen)
			{
				panelData.panel->OnEvent(e);
			}
		}
	}
}

void PanelManager::OnProjectChanged(const Shr<Project>& project)
{
	for (size_t i = 0; i < (size_t)PanelOpenSetting::_COUNT; i++)
	{
		for (const PanelData& panelData : panels[i] | std::views::values)
		{
			panelData.panel->OnProjectChanged(project);
		}
	}
}

void PanelManager::OnSceneChanged(const Shr<Scene>& scene)
{
	for (size_t i = 0; i < (size_t)PanelOpenSetting::_COUNT; i++)
	{
		for (const PanelData& panelData : panels[i] | std::views::values)
		{
			panelData.panel->OnSceneChanged(scene);
		}
	}
}

void PanelManager::RemovePanel(const std::string& strID)
{
	uint32_t id = Hash::GenerateFNVHash(strID);

	for (size_t i = 0; i < (size_t)PanelOpenSetting::_COUNT; i++)
	{
		if (auto& panelsInSetting = panels[i]; panelsInSetting.contains(id))
		{
			panelsInSetting.erase(id);
			return;
		}
	}
	LOG_CORE_ERROR("Could not remove panel: '{}'", strID);
}

void PanelManager::OpenPanel(const std::string& strID)
{
	uint32_t id = Hash::GenerateFNVHash(strID);

	for (size_t i = 0; i < (size_t)PanelOpenSetting::_COUNT; i++)
	{
		auto& panelsInSetting = panels[i];
		if (panelsInSetting.contains(id))
		{
			panelsInSetting[id].isOpen = true;
			return;
		}
	}
	LOG_CORE_ERROR("Could not open panel: '{}'", strID);
}
