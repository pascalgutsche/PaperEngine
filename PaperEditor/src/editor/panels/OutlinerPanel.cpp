#include "Editor.h"
#include "OutlinerPanel.h"

#include "editor/SelectionManager.h"

namespace PaperED
{
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

			auto texts = activeScene->Registry().view<TextComponent>();
			if (texts.size() && ImGui::TreeNode("Text"))
			{
				for (auto [entt, text] : texts.each()) {
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

	void OutlinerPanel::OnImGuiRender(bool& isOpen)
	{
		Shr<Scene> activeScene = Scene::GetActive();
		Entity selectedEntity = SelectionManager::GetSelection().ToEntity();
	
		UI::ScopedStyle min_width(ImGuiStyleVar_WindowMinSize, ImVec2(400.0f, 0.0f));
	
		ImGui::Begin(panelName.c_str(), &isOpen);
	
		if (activeScene)
		{
			Entities(activeScene, selectedEntity);
			TwoDObjects(activeScene, selectedEntity);
			Cameras(activeScene, selectedEntity);
		}
	
	
		ImGui::End();
		
	}
}
