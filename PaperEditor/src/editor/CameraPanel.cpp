#include "Editor.h"
#include "PELayer.h"

void PELayer::CameraMode()
{
	static bool active = true, previous_active = true;
	if (active != previous_active)
	{
		previous_active = active;
		return;
	}
	if (ImGui::BeginCombo("##1", ("Camera Mode: " + CameraModesToString(camera_mode)).c_str(), ImGuiComboFlags_HeightLargest))
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
		static std::array<CameraModes, 4> modes{ {Single, Split, Triple, Quadro} };
		for (auto& mode : modes)
		{
			if (mode == Single) {
				bool pop = false;
				if (mode == camera_mode)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
					pop = true;
				}
				if (ImGui::ImageButton("single", (void*)DataPool::GetTexture("main_view.png")->GetID(), ImVec2(75.0f, 50.0f)))
				{
					active = !active;
					camera_mode = mode;
					camera_mode_changed = true;
				}
				if (pop) ImGui::PopStyleColor();
				ImGui::SameLine();
				ImGui::Text("Single");
			}
			if (mode == Split) {
				bool pop = false;
				if (mode == camera_mode)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
					pop = true;
				}
				if (ImGui::ImageButton("split", (void*)DataPool::GetTexture("split_view.png")->GetID(), ImVec2(75.0f, 50.0f)))
				{
					active = !active;
					camera_mode = mode;
					camera_mode_changed = true;
				}
				if (pop) ImGui::PopStyleColor();
				ImGui::SameLine();
				ImGui::Text("Split");
			}
			if (mode == Triple) {
				bool pop = false;
				if (mode == camera_mode)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
					pop = true;
				}
				if (ImGui::ImageButton("triple", (void*)DataPool::GetTexture("triple_view.png")->GetID(), ImVec2(75.0f, 50.0f)))
				{
					active = !active;
					camera_mode = mode;
					camera_mode_changed = true;
				}
				if (pop) ImGui::PopStyleColor();
				ImGui::SameLine();
				ImGui::Text("Triple");
			}
			if (mode == Quadro) {
				bool pop = false;
				if (mode == camera_mode)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
					pop = true;
				}
				if (ImGui::ImageButton("quadro", (void*)DataPool::GetTexture("quadro_view.png")->GetID(), ImVec2(75.0f, 50.0f)))
				{
					active = !active;
					camera_mode = mode;
					camera_mode_changed = true;
				}
				if (pop) ImGui::PopStyleColor();
				ImGui::SameLine();
				ImGui::Text("Quadro");
			}
		}
		ImGui::PopStyleColor(3);
		ImGui::EndCombo();
	}
}

void PELayer::DockCameraPanel(CameraModes mode, ImGuiID main_id)
{
	ImGuiID top_id;
	ImGuiID bottom_id;
	ImGuiID left_id;
	ImGuiID right_id;
	ImGuiID top_left_id;
	ImGuiID top_right_id;
	ImGuiID bottom_left_id;
	ImGuiID bottom_right_id;

	ImGui::DockBuilderRemoveNode(main_id);
	ImGui::DockBuilderAddNode(main_id, ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(main_id, ImVec2(500, 500));

	switch (camera_mode)
	{
		case Split:
			ImGui::DockBuilderSplitNode(main_id, ImGuiDir_Left, 0.5f, &left_id, &right_id);
			break;
		case Triple:
			ImGui::DockBuilderSplitNode(main_id, ImGuiDir_Up, 0.5f, &top_id, &bottom_id);
			ImGui::DockBuilderSplitNode(top_id, ImGuiDir_Left, 0.5f, &top_left_id, &top_right_id);
			break;
		case Quadro:
			ImGui::DockBuilderSplitNode(main_id, ImGuiDir_Up, 0.5f, &top_id, &bottom_id);
			ImGui::DockBuilderSplitNode(top_id, ImGuiDir_Left, 0.5f, &top_left_id, &top_right_id);
			ImGui::DockBuilderSplitNode(bottom_id, ImGuiDir_Left, 0.5f, &bottom_left_id, &bottom_right_id);
			break;
		default: ;
	}
	ImGui::DockBuilderFinish(main_id);

	switch (camera_mode)
	{
	case Single:
		ImGui::DockBuilderDockWindow(viewports[0].name.c_str(), main_id);
		break;
	case Split:
		ImGui::DockBuilderDockWindow(viewports[0].name.c_str(), left_id);
		ImGui::DockBuilderDockWindow(viewports[1].name.c_str(), right_id);
		break;
	case Triple:
		ImGui::DockBuilderDockWindow(viewports[0].name.c_str(), top_left_id);
		ImGui::DockBuilderDockWindow(viewports[1].name.c_str(), top_right_id);
		ImGui::DockBuilderDockWindow(viewports[2].name.c_str(), bottom_id);
		break;
	case Quadro:
		ImGui::DockBuilderDockWindow(viewports[0].name.c_str(), top_left_id);
		ImGui::DockBuilderDockWindow(viewports[1].name.c_str(), top_right_id);
		ImGui::DockBuilderDockWindow(viewports[2].name.c_str(), bottom_left_id);
		ImGui::DockBuilderDockWindow(viewports[3].name.c_str(), bottom_right_id);
		break;
	}

}

void PELayer::EnableCamera(CameraModes mode)
{
	for (auto& port : viewports)
		port.is_visible = false;

	for (int i = 0; i <= mode; i++)
	{
		viewports[i].is_visible = true;
	}
}

void PELayer::CameraPanel(bool first, ImGuiWindowFlags& dock_flags)
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
	window_flags |= ImGuiWindowFlags_NoBackground;// | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	const char* name = "ViewPorts: ";

	if (first)
		DockPanel(name, GetDockspaceMain());

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin(name, nullptr, window_flags);
	ImGui::PopStyleVar(3);

	if (ImGui::BeginMenuBar())
	{
		ImGui::Text("");
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - 200);
		ImGui::PushItemWidth(200.0f);

		CameraMode();

		ImGui::PopItemWidth();
		ImGui::EndMenuBar();
	}

	ImGuiID dock_main_id = ImGui::GetID("ViewPort");
	ImGui::DockSpace(dock_main_id, ImVec2(0.0f, 0.0f), dock_flags);



	ImGui::End();

	for (auto& port : viewports)
		if (port.is_visible)
			port.Panel(this);

	if (camera_mode_changed)
	{
		EnableCamera(camera_mode);
		DockCameraPanel(camera_mode, dock_main_id);
		camera_mode_changed = false;
	}
}