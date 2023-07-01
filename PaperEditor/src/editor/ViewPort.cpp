#include "Editor.h"
#include "ViewPort.h"

#include "PELayer.h"

#include "util/Math.h"

#include <glm/gtx/matrix_decompose.hpp>

#include <ImGuizmo/ImGuizmo.h>

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
	framebuffer->ClearAttachment(1, -1);

	if (peLayer->scene)
		peLayer->scene->Render(camera);


	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

	//viewport_focused = ImGui::IsWindowFocused();
	viewport_hovered = ImGui::IsWindowHovered();

	if (viewport_focused)
		CameraMovement(camera);

	viewport_pos_abs = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };

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

	// Gizmos
	if (peLayer->active_entity) // && peLayer->GetGuizmoType() != -1
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		ImGuizmo::SetRect(viewport_bounds[0].x, viewport_bounds[0].y, viewport_bounds[1].x - viewport_bounds[0].x, viewport_bounds[1].y - viewport_bounds[0].y);

		// Camera

		// Runtime camera from entity
		// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
		// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
		// const glm::mat4& cameraProjection = camera.GetProjection();
		// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

		// Editor camera
		const glm::mat4& cameraProjection = camera->GetProjectionMatrix();
		glm::mat4 cameraView = camera->GetViewMatrix();

		// Entity transform
		auto& tc = peLayer->active_entity.GetComponent<TransformComponent>();
		glm::mat4 transform1 = tc.GetTransform();
		glm::mat4 transform(transform1);
		

		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
			ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(transform));


		if (ImGuizmo::IsUsing())
		{

			glm::vec3 position = glm::vec3(transform[3]);
			if (position.x != 0.0f)
			{
				int i = 0;
			}
			//ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));
			//
			//glm::vec3 deltaRotation = rotation - tc.rotation;
			tc.position = position;
			//tc.rotation += deltaRotation;
			//tc.scale = scale;
		}
	}

	//// Snapping
		//bool snap = Input::IsKeyPressed(KEY_LEFT_CONTROL);
		//float snapValue = 0.5f; // Snap to 0.5m for translation/scale
		//// Snap to 45 degrees for rotation
		//if (peLayer->GetGuizmoType() == ImGuizmo::OPERATION::ROTATE)
		//	snapValue = 45.0f;
		//
		//float snapValues[3] = { snapValue, snapValue, snapValue };

		//ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
		//	(ImGuizmo::OPERATION)peLayer->GetGuizmoType(), ImGuizmo::WORLD, glm::value_ptr(transform),
		//	nullptr, snap ? snapValues : nullptr);

	ImGui::End();
	ImGui::PopStyleVar();


	framebuffer->Unbind();
}