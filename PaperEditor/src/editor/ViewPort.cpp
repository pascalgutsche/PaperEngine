#include "Editor.h"
#include "ViewPort.h"

static void CameraMovement(const Shr<EditorCamera>& camera)
{
	const float dt = Application::GetDT();
	if (Input::IsKeyPressed(KEY_W))
	{
		camera->position.x += 5 * dt * camera->GetFront().x;
		camera->position.z += 5 * dt * camera->GetFront().z;
	}
	if (Input::IsKeyPressed(KEY_A))
	{
		camera->position.x += 5 * dt * camera->GetFront().z;
		camera->position.z -= 5 * dt * camera->GetFront().x;

	}
	if (Input::IsKeyPressed(KEY_S))
	{
		camera->position.x -= 5 * dt * camera->GetFront().x;
		camera->position.z -= 5 * dt * camera->GetFront().z;

	}
	if (Input::IsKeyPressed(KEY_D))
	{
		camera->position.x -= 5 * dt * camera->GetFront().z;
		camera->position.z += 5 * dt * camera->GetFront().x;

	}
	if (Input::IsKeyPressed(KEY_E))
		camera->position.y += 5 * dt;
	if (Input::IsKeyPressed(KEY_Q))
		camera->position.y -= 5 * dt;
}

void ViewPort::Panel(PELayer* peLayer)
{
	if (!open) return;
	if (FramebufferSpecification spec = framebuffer->GetSpecification();
		viewport_size.x > 0.0f && viewport_size.y > 0.0f && // zero sized framebuffer is invalid
		(spec.width != viewport_size.x || spec.height != viewport_size.y))
	{
		framebuffer->Resize((uint32_t)viewport_size.x, (uint32_t)viewport_size.y);

		camera->aspect_ratio = viewport_size.x / viewport_size.y;
	}

	framebuffer->Bind();

	RenderCommand::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	RenderCommand::Clear();
	framebuffer->ClearAttachment(1, 0);

	if (peLayer->scene)
		peLayer->scene->Render(camera);


	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin(name.c_str(), &open, ImGuiWindowFlags_NoCollapse);

	//viewport_focused = ImGui::IsWindowFocused();
	viewport_hovered = ImGui::IsWindowHovered();

	if (viewport_focused)
		CameraMovement(camera);

	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	viewport_bounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	viewport_bounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

	ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
	viewport_size = { viewport_panel_size.x, viewport_panel_size.y };

	uint32_t textureID = framebuffer->GetColorID(0);
	ImGui::Image((void*)textureID, ImVec2(viewport_size.x, viewport_size.y), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const wchar_t* path = (const wchar_t*)payload->Data;
			peLayer->new_scene = YAMLSerializer::SceneDeserialize(path);
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::End();
	ImGui::PopStyleVar();


	framebuffer->Unbind();
}