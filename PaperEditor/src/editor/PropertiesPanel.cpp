#include "Editor.h"
#include "PELayer.h"

#include "WindowsOpen.h"

void PELayer::PropertiesPanel()
{
	static bool first = true;

	ImGui::Begin("Properties", &show_property_panel);

	ImGui::End();

	first = false;
}