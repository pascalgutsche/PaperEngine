#include "Editor.h"
#include "PaperLayer.h"

#include <ImGuizmo/ImGuizmo.h>

#include "SelectionManager.h"

void PaperLayer::MousePicking()
{
	hovered_entity = Entity();
	ViewPort* port = nullptr;
	for (auto& viewport : viewports)
		if (viewport.is_visible && viewport.viewport_hovered)
			port = &viewport;
	if (!port) return;
	const ViewPort& viewPort = *port;

	auto [mx, my] = ImGui::GetMousePos();
	mx -= viewPort.viewport_bounds[0].x;
	my -= viewPort.viewport_bounds[0].y;
	glm::vec2 viewportSize = viewPort.viewport_bounds[1] - viewPort.viewport_bounds[0];
	my = viewportSize.y - my;

	glm::ivec2 mouse_pos{(int)mx, (int)my};

	if (!(mouse_pos.x >= 0 && mouse_pos.y >= 0 && mouse_pos.x < (int)viewportSize.x && mouse_pos.y < (int)viewportSize.y)) return; //mouse not inside any viewport

	viewPort.framebuffer->Bind();
	int pixelID = viewPort.framebuffer->ReadPixel(1, mouse_pos);
	viewPort.framebuffer->Unbind();

	pixelID > -1 ? hovered_entity = Entity((entt::entity)pixelID, Scene::GetActive().get()) : Entity();

	///TODO: MOVE SOMWHERE ELSE

	static Entity pressedEntity;

	if (Input::IsMouseButtonPressed(MouseButton::BUTTON_LEFT))
	{
		pressedEntity = hovered_entity;
	}

	if (Input::IsMouseButtonReleased(MouseButton::BUTTON_LEFT) && hovered_entity == Scene::GetActive()->GetEntity(SelectionManager::GetSelection()) && pressedEntity == hovered_entity && !drag_entity)
	{
		SelectionManager::Deselect();
	}
	else if (Input::IsMouseButtonReleased(MouseButton::BUTTON_LEFT) && !ImGuizmo::IsUsing() && !ImGuizmo::IsOver() && pressedEntity == hovered_entity && !drag_entity)
	{
		SelectionManager::Select(hovered_entity.GetPaperID());
	}

	if (Input::IsMouseButtonReleased(MouseButton::BUTTON_LEFT))
	{
		pressedEntity = Entity();
	}
}
