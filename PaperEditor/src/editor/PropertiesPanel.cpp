#include "Editor.h"
#include "PELayer.h"

#include "WindowsOpen.h"

#include <imgui/misc/cpp/imgui_stdlib.h>

static void DrawFloatControl(const std::string& label, float& value, float resetValue, float content_avail_x)
{
	float padding = 3.0f;
	float line_height = GImGui->Font->FontSize + padding * 2;
	ImVec2 button_size = { line_height + 3.0f, line_height };
	float input_item_width = (content_avail_x - 5.0f) / 3.0f - button_size.x;

	UI::ScopedID id(ImGui::TableGetRowIndex());

	if (ImGui::Button(label.c_str(), button_size))
	{
		value = resetValue;
	};
	ImGui::SameLine();
	ImGui::SetNextItemWidth(input_item_width);
	ImGui::DragFloat(("##" + label).c_str(), &value, 0.1f, 0, 0, "%.2f", 0);
}

static void Draw3FloatControl(const std::string& name, glm::vec3& val, float reset_value = 0.0f)
{
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	ImGui::TextUnformatted(name.c_str());
	//ImGui::Text(name.c_str());
	ImGui::TableSetColumnIndex(1);
	UI::ScopedStyle item_spacing(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });
	UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });

	float content_avail_x = ImGui::GetContentRegionAvail().x;



	DrawFloatControl("X", val.x, reset_value, content_avail_x);
	ImGui::SameLine();
	DrawFloatControl("Y", val.y, reset_value, content_avail_x);
	ImGui::SameLine();
	DrawFloatControl("Z", val.z, reset_value, content_avail_x);
}

void PELayer::PropertiesPanel()
{
	if (!active_entity) return;
	if (properties_panel_first)
		DockPanel("Properties", dock_id_right_bottom);

	UI::ScopedStyle min_width(ImGuiStyleVar_WindowMinSize, ImVec2(400.0f, 0.0f));

	ImGui::Begin("Properties", &show_property_panel);
	ImGui::Text("Name:");
	ImGui::SameLine();
	std::string name = active_entity.GetName();
	ImGui::InputText(("##" + active_entity.GetUUID().toString()).c_str(), &name);
	active_entity.SetName(name);

	if (active_entity.HasComponent<TransformComponent>())
	{
		auto& tc = active_entity.GetComponent<TransformComponent>();
		ImGui::BeginGroup();
		if ()
		if (ImGui::TreeNode("TransformComponent"))
		{
			ImGui::BeginTable("##dummy_id_transform", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoClip);
			ImGui::TableSetupColumn("name", 0, 100.0f);
			ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_IndentEnable | ImGuiTableColumnFlags_NoClip, ImGui::GetContentRegionAvail().x - 100.0f);

			

			
			Draw3FloatControl("Position", tc.position);
			Draw3FloatControl("Rotation", tc.rotation);
			Draw3FloatControl("Scale", tc.scale);

			ImGui::EndTable();

			ImGui::TreePop();
		}
		ImGui::EndGroup();

	}

	ImGui::End();

	properties_panel_first = false;
}