#include "Editor.h"
#include "ViewPort.h"

#include "PaperLayer.h"

#include "util/Math.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <gtx/string_cast.hpp>

#include <ImGuizmo/ImGuizmo.h>

#include "SelectionManager.h"

static constexpr float normalSpeed = 5.0f;

static float GetCameraSpeed()
{
	float speed = normalSpeed;
	if (Input::IsKeyDown(Key::LEFT_CONTROL))
		speed /= 8 - glm::log(normalSpeed);
	if (Input::IsKeyDown(Key::LEFT_SHIFT))
		speed *= 8 - glm::log(normalSpeed);

	return glm::clamp(speed, 2.0f, 10.0f);
}

static void CameraMovement(const Ref<EditorCamera>& camera)
{
	const float dt = Application::GetDT();
	const float speed = GetCameraSpeed();
	if (Input::IsKeyDown(Key::W))
	{
		camera->position.x += speed * dt * camera->GetFront().z;
		camera->position.z -= speed * dt * camera->GetFront().x;
	}
	if (Input::IsKeyDown(Key::A))
	{
		camera->position.x -= speed * dt * camera->GetFront().x;
		camera->position.z -= speed * dt * camera->GetFront().z;
	}
	if (Input::IsKeyDown(Key::S))
	{
		camera->position.x -= speed * dt * camera->GetFront().z;
		camera->position.z += speed * dt * camera->GetFront().x;
	}
	if (Input::IsKeyDown(Key::D))
	{
		camera->position.x += speed * dt * camera->GetFront().x;
		camera->position.z += speed * dt * camera->GetFront().z;
	}
	if (Input::IsKeyDown(Key::E))
		camera->position.y += speed * dt;
	if (Input::IsKeyDown(Key::Q))
		camera->position.y -= speed * dt;
}

#include "renderer/Renderer2D.h"
void ViewPort::Panel(PaperLayer* peLayer)
{
	const Ref<Scene> activeScene = Scene::GetActive();

	if (activeScene && viewport_size.x > 0.0f && viewport_size.y > 0.0f)
		activeScene->OnViewportResize(viewport_size.x, viewport_size.y);

	if (FramebufferSpecification spec = framebuffer->GetSpecification();
		viewport_size.x > 0.0f && viewport_size.y > 0.0f && // zero sized framebuffer is invalid
		(spec.width != viewport_size.x || spec.height != viewport_size.y))
	{
		const Ref<Scene> activeScene = Scene::GetActive();
		framebuffer->Resize((uint32_t)viewport_size.x, (uint32_t)viewport_size.y);

		camera->aspect_ratio = viewport_size.x / viewport_size.y;
		//if (activeScene)
		//	activeScene->OnViewportResize(viewport_size.x, viewport_size.y);
	}

	RenderCommand::ClearColor(glm::vec4(0.0f));
	RenderCommand::Clear();

	framebuffer->Bind();
	
	RenderCommand::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	RenderCommand::Clear();
	framebuffer->ClearAttachment(1, -1);

	RenderCommand::ClearStats();

	if (activeScene)
	{
		switch (peLayer->sceneState)
		{
		case SceneState::Edit:
			activeScene->OnEditorUpdate(camera);
			Renderer2D::BeginRender(camera);
			for (PaperID id : SelectionManager::GetSelections(SelectionManagerType::ViewPort))
			{
				if (id.ToEntity())
				Renderer2D::DrawLineRect(id.ToEntity().GetComponent<TransformComponent>().GetTransform(), glm::vec4(1, 0.459, 0.004, 1.0), (uint32_t)id.ToEntity());
			}
			Renderer2D::EndRender();
			break;
		case SceneState::Play:
			activeScene->OnRuntimeUpdate();
			break;
		case SceneState::Simulate:
			activeScene->OnSimulationUpdate(camera);
			break;
		}
	}
	framebuffer->Unbind();

	if (peLayer->lastFocusedViewPort == this)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2, 2));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.6f, 0.6f, 1.0f, 1.0f));
	}
	else
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	
	
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

	ImGui::PopStyleVar();
	if (peLayer->lastFocusedViewPort == this)
		ImGui::PopStyleColor();

	viewport_focused = ImGui::IsWindowFocused();
	if (viewport_focused) peLayer->lastFocusedViewPort = this;
	viewport_hovered = ImGui::IsWindowHovered();

	if (viewport_active)
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
	ImGui::SetItemAllowOverlap();

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const wchar_t* path = (const wchar_t*)payload->Data;
			peLayer->OpenScene(SceneSerializer::Deserialize(path));
		}
		ImGui::EndDragDropTarget();
	}

	//entityCamera preview
	if (activeScene && peLayer->sceneState != SceneState::Play)
	{
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMax().x - previewPadding.x - previewSize.x, ImGui::GetWindowContentRegionMax().y - previewPadding.y - previewSize.y));

		previewFramebuffer->Bind();
		RenderCommand::ClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		RenderCommand::Clear();
		activeScene->OnRuntimeRender(previewEntityID);
		previewFramebuffer->Unbind();

		uint32_t previewTextureID = previewFramebuffer->GetColorID(0);
		ImGui::Image((void*)previewTextureID, previewSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY"))
			{
				PaperID entityID = *(uint64_t*)payload->Data;
				previewEntityID = entityID;
			}
			ImGui::EndDragDropTarget();
		}
	}
	

	

	// Gizmos
	if (SelectionManager::HasSelection(SelectionManagerType::ViewPort) && peLayer->sceneState == SceneState::Edit) //  && peLayer->GetGuizmoType() != -1   
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist(ImGui::GetCurrentWindow()->DrawList);

		ImGuizmo::SetRect(viewport_bounds[0].x, viewport_bounds[0].y, viewport_bounds[1].x - viewport_bounds[0].x, viewport_bounds[1].y - viewport_bounds[0].y);

		// Editor camera
		const glm::mat4& cameraProjection = camera->GetProjectionMatrix();
		glm::mat4 cameraView = camera->GetViewMatrix();

		// Entity transform
		auto& tc = SelectionManager::GetSelections(SelectionManagerType::ViewPort)[0].ToEntity().GetComponent<TransformComponent>();
		glm::mat4 transform = tc.GetTransform();

		// Snapping
		bool snap = Input::IsKeyDown(Key::LEFT_CONTROL);
		float snapValue = 0.5f; // Snap to 0.5m for translation/scale
		// Snap to 45 degrees for rotation
		if (peLayer->GetGuizmoType() == ImGuizmo::OPERATION::ROTATE)
			snapValue = 45.0f;

		float snapValues[3] = { snapValue, snapValue, snapValue };

		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
			(ImGuizmo::OPERATION)peLayer->GetGuizmoType(), ImGuizmo::LOCAL, glm::value_ptr(transform),
			nullptr, snap ? snapValues : nullptr);

		if (ImGuizmo::IsUsing())
		{
			glm::vec3 translation, rotation, scale;
			Math::DecomposeTransform(transform, translation, rotation, scale);

			glm::vec3 deltaRotation = rotation - glm::radians(tc.rotation);
			tc.position = translation;
			tc.rotation += glm::degrees(deltaRotation);
			tc.scale = scale;
		}
	}

	ImGui::End();



}

void ViewPort::Resize()
{
	
}
