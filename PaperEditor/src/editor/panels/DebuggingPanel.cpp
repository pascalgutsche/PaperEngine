#include "Editor.h"

#include "imgui/UI.h"
#include "DebuggingPanel.h"

#include "editor/PaperLayer.h"
#include "editor/SelectionManager.h"

void ApplicationPanel::OnImGuiRender(bool& isOpen)
{
	const float dt = Application::GetDT();
	ImGui::Begin(panelName.c_str(), &isOpen);

	static float time = 0;
	static float timehelper = -1;
	static float history = 5;
	static int flags = ImPlotAxisFlags_NoTickLabels;
	static ScrollingBuffer sbuff_dt(300);

	if (timehelper >= 0.016f || timehelper == -1) {
		timehelper = 0;
		sbuff_dt.AddPoint(time, 1000 * dt);
	}
	timehelper += dt;
	time += dt;

	std::string entity_name1 = "None";
	//if (hovered_entity)
	//	entity_name1 = hovered_entity.GetName();
	//ImGui::Text("Hovered Entity: %s", entity_name1.c_str());

	std::string entity_name2 = "None";
	if (SelectionManager::HasSelection())
		entity_name2 = Scene::GetActive()->GetEntity(SelectionManager::GetSelection()).GetName();
	ImGui::Text("Active Entity: %s", entity_name2.c_str());
	std::stringstream stream;
	if (ImGui::TreeNode("Time"))
	{
		ImGui::SliderFloat("Time", &history, 1, 5, "%.1f");

		stream << "ms per frame: " << 1000 * dt;
		ImGui::Text(stream.str().c_str()); stream.str("");
		if (ImPlot::BeginPlot("##ms_per_frame", ImVec2(-1, 100))) {
			ImPlot::SetupAxes(NULL, NULL, flags, flags);
			ImPlot::SetupAxisLimits(ImAxis_X1, time - history, time, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 20);
			ImPlot::SetNextFillStyle(ImVec4(0.0f, 0.0f, 1.0f, -1.0f), 0.5f);
			ImPlot::PlotLine("##ms", &sbuff_dt.Data[0].x, &sbuff_dt.Data[0].y, sbuff_dt.Data.size(), 0, sbuff_dt.Offset, 2 * sizeof(float));
			ImPlot::EndPlot();
		}

		stream << "frames per sec: " << 1 / dt;
		ImGui::Text(stream.str().c_str()); stream.str("");

		static bool vsync = true;
		ImGui::Checkbox("V-Sync", &vsync);
		Application::GetWindow()->SetVSync(vsync);



		ImGui::Text("");
		ImGui::TreePop();
	}


	if (ImGui::TreeNode("Render Stats"))
	{
		stream << "Draw calls: " << RenderCommand::GetStats().drawCalls;
		ImGui::BulletText(stream.str().c_str()); stream.str("");

		stream << "Object count: " << RenderCommand::GetStats().objectCount;
		ImGui::BulletText(stream.str().c_str()); stream.str("");

		stream << "Data size: " << RenderCommand::GetStats().dataSize << " Bytes";
		ImGui::BulletText(stream.str().c_str()); stream.str("");

		stream << "Vertex count: " << RenderCommand::GetStats().vertexCount;
		ImGui::BulletText(stream.str().c_str()); stream.str("");

		stream << "Indices count: " << RenderCommand::GetStats().elementCount;
		ImGui::BulletText(stream.str().c_str()); stream.str("");

		ImGui::Text("");

		stream << "Polygon Model: ";
		ImGui::Text(stream.str().c_str()); stream.str("");

		static int selected = 0;
		if (ImGui::Selectable("OFF", selected == 0))
			RenderCommand::SetPolygonModel(Polygon::OFF);
		if (ImGui::Selectable("LINE", selected == 2))
			RenderCommand::SetPolygonModel(Polygon::LINE);
		if (ImGui::Selectable("POINT", selected == 1))
			RenderCommand::SetPolygonModel(Polygon::POINT);



		ImGui::BeginDisabled();
		if (ImGui::TreeNode("Textures in use"))
		{
			/*
			for (Shr<Texture> texture : Renderer2D::GetTexturesInUse())
			{

				ImGui::Selectable(texture->GetName().c_str(), false);

			}
			*/
			ImGui::Text("");
			ImGui::TreePop();
		}
		ImGui::EndDisabled();


		ImGui::TreePop();
	}
	ImGui::BeginDisabled();
	if (ImGui::TreeNode("Camera"))
	{
		//if (ImGui::Button("Reset Camera"))
		//{
		//	Application::GetActiveScene()->GetCamera()->fov = 1.0f;
		//	Application::GetActiveScene()->GetCamera()->position = glm::vec3(0.0f, 0.0f, 5.0f);
		//}
		//
		//glm::vec3 pos = Application::GetActiveScene()->GetCamera()->position;
		//float arrayPos[]{ pos.x, pos.y, pos.z };
		//ImGui::DragFloat3("XYZ", arrayPos, 0.5f);
		//Application::GetActiveScene()->GetCamera()->position = glm::vec3(arrayPos[0], arrayPos[1], arrayPos[2]);
		//
		//float fov = Application::GetActiveScene()->GetCamera()->fov;
		//ImGui::DragFloat("FOV", &fov, 0.5f);
		//Application::GetActiveScene()->GetCamera()->fov = fov;
		//
		ImGui::TreePop();
	}
	ImGui::EndDisabled();

	ImGui::End();
}

void ViewportDebuggingPanel::OnImGuiRender(bool& isOpen)
{
	ImGui::Begin(panelName.c_str(), &isOpen);

	ImGui::Text(("Last focused Viewport: " + paperLayer->lastFocusedViewPort->name).c_str());

	ImGui::Separator();

	for (auto& viewport : paperLayer->viewports)
	{
		ImGui::Text(viewport.name.c_str());
		ImGui::Checkbox("is_visible", &viewport.is_visible);
		ImGui::Checkbox("last_viewport_focused", &viewport.last_viewport_active);
		ImGui::Checkbox("viewport_focused", &viewport.viewport_focused);
		ImGui::Checkbox("viewport_hovered", &viewport.viewport_hovered);
		ImGui::Checkbox("viewport_active", &viewport.viewport_active);

		ImGui::InputFloat2("viewport_bounds[0]", &viewport.viewport_bounds[0].x);
		ImGui::InputFloat2("viewport_bounds[1]", &viewport.viewport_bounds[1].x);
		ImGui::InputFloat2("viewport_size", &viewport.viewport_size.x);
		ImGui::InputFloat2("viewport_pos_abs", &viewport.viewport_pos_abs.x);

	}

	ImGui::End();
}

void SceneDebuggingPanel::OnImGuiRender(bool& isOpen)
{
	const Shr<Scene> activeScene = Scene::GetActive();
	ImGui::Begin(panelName.c_str(), &isOpen);

	if (!activeScene)
	{
		ImGui::Text("no activeScene active!");
		ImGui::End();
		return;
	}

	auto view = activeScene->Registry().view<TransformComponent>();
	int i = 0;
	for (auto [entity, transform] : view.each()) {
		ImGui::PushID(i);
		ImGui::Text(Entity(entity, activeScene.get()).GetName().c_str());
		ImGui::InputFloat3("Position", &transform.position.x);
		ImGui::InputFloat3("Scale", &transform.scale.x);
		ImGui::InputFloat3("Rotation", &transform.rotation.x);

		ImGui::Text("-----------------------");
		ImGui::PopID();
		i++;
	}

	ImGui::End();
}

void CameraSettingsPanel::OnImGuiRender(bool& isOpen)
{
	ImGui::Begin("Camera Settings", &isOpen, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::BeginTabBar("##tabs"))
	{
		for (auto& viewport : paperLayer->viewports)
		{
			if (ImGui::BeginTabItem(viewport.name.c_str()))
			{
				const Shr<EditorCamera> camera = viewport.camera;

				ImGui::DragFloat3("Position", &camera->position.x, 0.1f);
				ImGui::DragFloat("Yaw", &camera->yaw, 0.1f);
				ImGui::DragFloat("Pitch", &camera->pitch, 0.1f);
				ImGui::DragFloat("Roll", &camera->roll, 0.1f);

				ImGui::DragFloat("FOV", &camera->fov, 0.1f);

				ImGui::DragFloat("Far-Plane", &camera->far_plane, 0.01f);
				ImGui::DragFloat("Near-Plane", &camera->near_plane, 0.01f);
				ImGui::DragFloat("Left_Plane", &camera->left_frostum, 0.01f);
				ImGui::DragFloat("Right-Plane", &camera->right_frostum, 0.01f);
				ImGui::DragFloat("Top-Plane", &camera->top_frostum, 0.01f);
				ImGui::DragFloat("Bottom-Plane", &camera->bottom_frostum, 0.01f);

				ImGui::DragFloat3("Target", &camera->target.x, 0.1f);


				if (ImGui::Button("Reset Camera Values"))
					camera->ResetValues();

				ImGui::EndTabItem();

			}
		}


		ImGui::EndTabBar();
	};


	ImGui::End();


	ImGui::Begin("loool");

	if (UI::BeginImageTreeNode("lol", DataPool::GetTexture("resources/editor/icons/gear.png", true), {14, 14}))
	{
		UI::BeginPropertyGrid();

		//BOOL SEC
		static bool boolVar = false;
		UI::Property("bool", boolVar);

		
		//FLOAT SEC
		static float float1 = 1.0f;
		UI::Property("float1", float1);

		static glm::vec2 float2 = glm::vec2(1.0f);
		UI::Property("float2", float2);

		static glm::vec3 float3 = glm::vec3(1.0f);
		UI::Property("float3", float3);

		static glm::vec4 float4 = glm::vec4(1.0f);
		UI::Property("float4", float4);

		//INT SEC
		static int int1 = 1.0f;
		UI::Property("int1", int1);

		static glm::ivec2 int2 = glm::ivec2(1.0f);
		UI::Property("int2", int2);

		static glm::ivec3 int3 = glm::ivec3(1.0f);
		UI::Property("int3", int3);

		static glm::ivec4 int4 = glm::ivec4(1.0f);
		UI::Property("int4", int4);

		//STRING SEC
		static std::string string1 = "bunker";
		UI::Property("string1", string1);

		UI::EndPropertyGrid();

		UI::EndTreeNode();
	}

	//UI::BeginPropertyGrid();
	//static bool val = false;
	//UI::Property("Chkdsfhdjkdjkghdfjjheckbox", val);
	//static bool val1 = false;
	//UI::Property("Chkdsfhdjkdjkghdfjjheckbox", val1);
	//
	//if (UI::PropertyTreeNode("lol", true))
	//{
	//	ImGui::Text("Bunker");
	//	ImGui::TreePop();
	//}
	//
	//UI::EndPropertyGrid();

	ImGui::End();

}

void ImGuiDemoPanel::OnImGuiRender(bool& isOpen)
{
	ImGui::ShowDemoWindow(&isOpen);
}
