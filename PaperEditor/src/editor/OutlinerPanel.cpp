#include "Editor.h"
#include "PELayer.h"

#include "WindowsOpen.h"

void TwoDObjects(Shr<Scene>& scene, Entity& active_entity)
{
	if (ImGui::TreeNode("2D-Objects"))
	{
		auto sprites_s = scene->Registry().view<SpriteComponent>();
		if (!sprites_s.empty() && ImGui::TreeNode("Sprite"))
		{
			for (auto [entt, sprite] : sprites_s.each()) {
				Entity entity(entt, scene.get());
				if (ImGui::Selectable(entity.GetName().c_str())) {
					active_entity = entity;
				}
			}
			ImGui::TreePop();
		}

		auto lines = scene->Registry().view<LineComponent>();
		if (lines.size() && ImGui::TreeNode("Line"))
		{
			for (auto [entt, line] : lines.each()) {
				Entity entity(entt, scene.get());
				if (ImGui::Selectable(entity.GetName().c_str())) {
					active_entity = entity;
				}
			}
			ImGui::TreePop();
		}

		auto text = scene->Registry().view<TextComponent>();
		if (text.size() && ImGui::TreeNode("Text"))
		{
			for (auto [entt, text] : text.each()) {
				Entity entity(entt, scene.get());
				if (ImGui::Selectable(entity.GetName().c_str())) {
					active_entity = entity;
				}
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

void PELayer::OutlinerPanel()
{
	std::string name = "Outliner";

	if (outliner_panel_first)
		DockPanel(name, dock_id_right);

	UI::ScopedStyle min_width(ImGuiStyleVar_WindowMinSize, ImVec2(400.0f, 0.0f));

	ImGui::Begin(name.c_str(), &show_outliner_panel);

	if (scene)
	{
		TwoDObjects(scene, active_entity);
	}


	ImGui::End();

	outliner_panel_first = false;
}