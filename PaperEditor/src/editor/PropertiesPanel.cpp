#include "Editor.h"
#include "PELayer.h"

#include "WindowsOpen.h"

void PELayer::PropertiesPanel()
{

	ImGui::Begin("Properties", &show_property_panel);

	ImGui::End();

	properties_panel_first = false;
}