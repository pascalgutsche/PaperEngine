#pragma once
#include "Engine.h"
#include "imgui/ImGuiLayer.h"
#include "imgui/ImGuiUtils.h"
#include "utils/Utils.h"

namespace Paper::UI
{

	inline bool CheckBox(const std::string& name, bool& val)
	{
		const bool moddified = ImGui::Checkbox(name.c_str(), &val);
		UI::DrawItemActivityOutline();
		return moddified;
	}

	struct FloatControlSettings
	{
		float speed = 1;
		float min = 0;
		float max = 0;
		std::string format = "%.3f";
		ImGuiSliderFlags flags = ImGuiSliderFlags_None;
	};

	inline bool FloatControl(std::string name, float& val, FloatControlSettings settings = {})
	{
		const bool modified = ImGui::DragFloat(name.c_str(), &val, settings.speed, settings.min, settings.max, settings.format.c_str(), settings.flags);
		UI::DrawItemActivityOutline();
		return modified;
	}

	struct IntControlSettings
	{
		float speed = 1;
		float min = 0;
		float max = 0;
		std::string format = "%d";
		ImGuiSliderFlags flags = ImGuiSliderFlags_None;
	};

	inline bool IntControl(std::string name, int& val, IntControlSettings settings = {})
	{
		const bool modified = ImGui::DragInt(name.c_str(), &val, settings.speed, settings.min, settings.max, settings.format.c_str(), settings.flags);
		UI::DrawItemActivityOutline();
		return modified;
	}

	inline bool StringControl(std::string name, std::string& val)
	{
		const bool modified = ImGui::InputText(name.c_str(), &val);
		UI::DrawItemActivityOutline();
		return modified;
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

	inline bool BeginTreeNode(std::string label, bool defaultOpen = false)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (defaultOpen)
			flags |= ImGuiTreeNodeFlags_DefaultOpen;

		return ImGui::TreeNodeEx(label.c_str(), flags);
	}

	inline bool BeginImageTreeNode(std::string label, Shr<Texture> image = nullptr, ImVec2 size = { 0.0f, 0.0f }, bool defaultOpen = true)
	{
		Shr<Texture> gearTexture = DataPool::GetTexture("resources/editor/icons/gear.png", true);

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;
		if (defaultOpen)
			flags |= ImGuiTreeNodeFlags_DefaultOpen;


		ScopedStyle frameRounding(ImGuiStyleVar_FrameRounding, 2.0f);
		ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2( 6.0f, 6.0f ));
		ScopedStyle cellPadding(ImGuiStyleVar_CellPadding, ImVec2( 6.0f, 60.0f ));

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

	inline void BeginPropertyElementInternal(const std::string& label)
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);

		ImGui::PushID(label.c_str());

		UI::ShiftCursorY(8);
		ImGui::AlignTextToFramePadding();
		ImGui::Text(label.c_str());
		UI::ShiftCursorY(8);

		ImGui::TableNextColumn();

		UI::ShiftCursorY(6);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
	}

	inline void EndPropertyElementInternal()
	{
		ImGui::PopStyleVar();
		UI::ShiftCursorY(6);

		ImGui::PopID();

		ImGui::TableSetColumnIndex(0);
	}

	inline bool Property(const std::string& label, bool& val)
	{
		UI::BeginPropertyElementInternal(label);

		const bool modified = CheckBox("", val);

		UI::EndPropertyElementInternal();

		return modified;
	}

	inline bool Property(const std::string& label, float& val)
	{
		UI::BeginPropertyElementInternal(label);

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool modified = FloatControl("", val);

		UI::EndPropertyElementInternal();

		return modified;
	}

	inline bool Property(const std::string& label, glm::vec2& val)
	{
		constexpr int c = 2;

		UI::BeginPropertyElementInternal(label);

		float wigetWidth = ImGui::GetContentRegionAvail().x / c - 2;

		bool modified = false;

		for (int i = 0; i < c; i++)
		{
			if (i > 0)
			{
				ImGui::SameLine();
				UI::ShiftCursorX(4);
			}

			ImGui::SetNextItemWidth(wigetWidth);
			UI::ScopedID id(i);
			modified |= FloatControl("", val[i]);
		}

		UI::EndPropertyElementInternal();

		return modified;
	}

	inline bool Property(const std::string& label, glm::vec3& val)
	{
		constexpr int c = 3;

		UI::BeginPropertyElementInternal(label);

		float wigetWidth = ImGui::GetContentRegionAvail().x / c - 2;

		bool modified = false;

		for (int i = 0; i < c; i++)
		{
			if (i > 0)
			{
				ImGui::SameLine();
				UI::ShiftCursorX(4);
			}

			ImGui::SetNextItemWidth(wigetWidth);
			UI::ScopedID id(i);
			modified |= FloatControl("", val[i]);
		}

		UI::EndPropertyElementInternal();

		return modified;
	}

	inline bool Property(const std::string& label, glm::vec4& val)
	{
		constexpr int c = 4;

		UI::BeginPropertyElementInternal(label);

		float wigetWidth = ImGui::GetContentRegionAvail().x / c - 3;

		bool modified = false;

		for (int i = 0; i < c; i++)
		{
			if (i > 0)
			{
				ImGui::SameLine();
				UI::ShiftCursorX(4);
			}

			ImGui::SetNextItemWidth(wigetWidth);
			UI::ScopedID id(i);
			modified |= FloatControl("", val[i]);
		}

		UI::EndPropertyElementInternal();

		return modified;
	}

	inline bool Property(const std::string& label, int& val)
	{
		UI::BeginPropertyElementInternal(label);

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool modified = IntControl("", val);

		UI::EndPropertyElementInternal();

		return modified;
	}

	inline bool Property(const std::string& label, glm::ivec2& val)
	{
		constexpr int c = 2;

		UI::BeginPropertyElementInternal(label);

		float wigetWidth = ImGui::GetContentRegionAvail().x / c - 2;

		bool modified = false;

		for (int i = 0; i < c; i++)
		{
			if (i > 0)
			{
				ImGui::SameLine();
				UI::ShiftCursorX(4);
			}

			ImGui::SetNextItemWidth(wigetWidth);
			UI::ScopedID id(i);
			modified |= IntControl("", val[i]);
		}

		UI::EndPropertyElementInternal();

		return modified;
	}

	inline bool Property(const std::string& label, glm::ivec3& val)
	{
		constexpr int c = 3;

		UI::BeginPropertyElementInternal(label);

		float wigetWidth = ImGui::GetContentRegionAvail().x / c - 2;

		bool modified = false;

		for (int i = 0; i < c; i++)
		{
			if (i > 0)
			{
				ImGui::SameLine();
				UI::ShiftCursorX(4);
			}

			ImGui::SetNextItemWidth(wigetWidth);
			UI::ScopedID id(i);
			modified |= IntControl("", val[i]);
		}

		UI::EndPropertyElementInternal();

		return modified;
	}

	inline bool Property(const std::string& label, glm::ivec4& val)
	{
		constexpr int c = 4;

		UI::BeginPropertyElementInternal(label);

		float wigetWidth = ImGui::GetContentRegionAvail().x / c - 3;

		bool modified = false;

		for (int i = 0; i < c; i++)
		{
			if (i > 0)
			{
				ImGui::SameLine();
				UI::ShiftCursorX(4);
			}

			ImGui::SetNextItemWidth(wigetWidth);
			UI::ScopedID id(i);
			modified |= IntControl("", val[i]);
		}

		UI::EndPropertyElementInternal();

		return modified;
	}

	inline bool Property(const std::string& label, std::string& val)
	{
		UI::BeginPropertyElementInternal(label);

		float wigetWidth = ImGui::GetContentRegionAvail().x;

		bool modified = false;

		{
			ImGui::SetNextItemWidth(wigetWidth);
			UI::ScopedID id(0);
			modified |= StringControl("", val);
		}


		UI::EndPropertyElementInternal();

		return modified;
	}

	

	
}
