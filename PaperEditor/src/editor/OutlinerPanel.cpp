#include "Editor.h"
#include "PaperLayer.h"

#include "WindowsOpen.h"

void Entities(Shr<Scene>& activeScene, Entity& active_entity)
{
	if (ImGui::TreeNode("Entities"))
	{
		auto entities = activeScene->Registry().view<TransformComponent>();
		for (auto entt : entities) {
			Entity entity(entt, activeScene.get());
			if (ImGui::Selectable(entity.GetName().c_str())) {
				active_entity = entity;
			}
		}

		ImGui::TreePop();
	}
}

void TwoDObjects(Shr<Scene>& activeScene, Entity& active_entity)
{
	if (ImGui::TreeNode("2D-Object"))
	{
		auto sprites_s = activeScene->Registry().view<SpriteComponent>();
		if (!sprites_s.empty() && ImGui::TreeNode("Sprite"))
		{
			for (auto [entt, sprite] : sprites_s.each()) {
				Entity entity(entt, activeScene.get());
				if (ImGui::Selectable(entity.GetName().c_str())) {
					active_entity = entity;
				}
			}
			ImGui::TreePop();
		}

		auto lines = activeScene->Registry().view<LineComponent>();
		if (lines.size() && ImGui::TreeNode("Line"))
		{
			for (auto [entt, line] : lines.each()) {
				Entity entity(entt, activeScene.get());
				if (ImGui::Selectable(entity.GetName().c_str())) {
					active_entity = entity;
				}
			}
			ImGui::TreePop();
		}

		auto text = activeScene->Registry().view<TextComponent>();
		if (text.size() && ImGui::TreeNode("Text"))
		{
			for (auto [entt, text] : text.each()) {
				Entity entity(entt, activeScene.get());
				if (ImGui::Selectable(entity.GetName().c_str())) {
					active_entity = entity;
				}
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

void Cameras(Shr<Scene>& activeScene, Entity& active_entity)
{
	if (ImGui::TreeNode("Cameras"))
	{
		auto cameras = activeScene->Registry().view<CameraComponent>();
		for (auto entt : cameras) {
			Entity entity(entt, activeScene.get());
			if (ImGui::Selectable(entity.GetName().c_str())) {
				active_entity = entity;
			}
		}

		ImGui::TreePop();
	}
}

void PaperLayer::OutlinerPanel()
{
	std::string name = "Outliner";

	if (outliner_panel_first)
		DockPanel(name, dock_id_right);

	UI::ScopedStyle min_width(ImGuiStyleVar_WindowMinSize, ImVec2(400.0f, 0.0f));

	ImGui::Begin(name.c_str(), &show_outliner_panel);

	if (activeScene)
	{
		Entities(activeScene, active_entity);
		TwoDObjects(activeScene, active_entity);
		Cameras(activeScene, active_entity);
	}


	ImGui::End();

	outliner_panel_first = false;
}