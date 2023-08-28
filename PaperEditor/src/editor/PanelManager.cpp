#include "Editor.h"
#include "PanelManager.h"

#include "DockManager.h"

void PanelManager::OnImGuiRender()
{
	for (PanelData& panelData : panels | std::views::values)
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
