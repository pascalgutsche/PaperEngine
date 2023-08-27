#include "Editor.h"
#include "PanelManager.h"

void PanelManager::OnImGuiRender()
{
	for (PanelData& panelData : panels | std::views::values)
	{
		if (panelData.isOpen)
		{
			panelData.panel->OnImGuiRender(panelData.isOpen, panelData.firstRender);
			panelData.firstRender = false;
		}
	}
}

void PanelManager::OnEvent(Event& e)
{
	for (const PanelData& panelData : panels | std::views::values)
	{
		if (panelData.isOpen)
		{
			panelData.panel->OnEvent(e);
		}
	}
}
