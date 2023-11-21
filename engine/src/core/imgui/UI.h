#pragma once
#include "Engine.h"
#include "imgui/ImGuiLayer.h"
#include "imgui/ImGuiUtils.h"
#include "utils/Utils.h"

namespace Paper::UI
{

	inline bool CheckBox(std::string name, bool& val)
	{
		bool checked = ImGui::Checkbox(name.c_str(), &val);
		UI::DrawItemActivityOutline();
		return checked;
	}


	inline void BeginPropertyGrid()
	{
		PushID();
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::BeginTable(GenerateID(), 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable); //
		ImGui::TableSetupColumn("name", 0); // ImGui::GetContentRegionAvail().x / 3
		//float space_left = space_name_column > 0 ? ImGui::GetContentRegionAvail().x - space_name_column : 0;
		ImGui::TableSetupColumn("value"); //, ImGuiTableColumnFlags_IndentEnable | ImGuiTableColumnFlags_NoClip
		
	}

	inline void EndPropertyGrid()
	{

		ImGui::EndTable();
		ImGui::PopStyleVar(4);

		PopID();
	}


	inline bool Property(const std::string& label, bool& val)
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);

		ImGui::PushID(label.c_str());
		ImGui::AlignTextToFramePadding();
		ImGui::Text(label.c_str());

		ImGui::TableNextColumn();

		ImGui::PushItemWidth(-1);
		bool activated = CheckBox("", val);
		ImGui::PopItemWidth();
		ImGui::PopID();
		ImGui::TableSetColumnIndex(0);

		return activated;
	}

	inline bool BeginTreeNode(std::string label, bool defaultOpen = false)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (defaultOpen)
			flags |= ImGuiTreeNodeFlags_DefaultOpen;

		return ImGui::TreeNodeEx(label.c_str(), flags);
	}

	inline bool BeginImageTreeNode(std::string label, Shr<Texture> image = nullptr, ImVec2 size = { 0.0f, 0.0f }, bool defaultOpen = false)
	{
		Shr<Texture> gearTexture = DataPool::GetTexture("resources/editor/icons/gear.png", true);

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;
		if (defaultOpen)
			flags |= ImGuiTreeNodeFlags_DefaultOpen;


		ScopedStyle frameRounding(ImGuiStyleVar_FrameRounding, 2.0f);
		ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2( 6.0f, 6.0f ));

		ImGui::PushID(label.c_str());
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
		bool open = ImGui::TreeNodeEx("##dummy_id", flags, "");

		float lineHeight = ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y;

		if (size.x == 0.0f || size.y == 0.0f)
		{
			size.x = lineHeight;
			size.y = lineHeight;
		}

		if (image)
		{
			ImGui::SameLine();
			float yshift = (lineHeight / 2) - (size.y / 2);
			UI::ShiftCursorY(yshift);
			ImGui::Image((void*)image->GetID(), size);
		}
		
		ImGui::SameLine();
		//UI::ShiftCursorY(-yshift);
		ImGui::TextUnformatted(Utils::ToUpper(label).c_str());

		ImGui::PopID();
			

		return open;

	}

	inline void EndTreeNode()
	{
		ImGui::TreePop();
	}

	inline void EndPropertyTreeNode()
	{
		ImGui::TreePop();
	}

	inline bool PropertyImageTreenode()
	{
		
	}

	
}
