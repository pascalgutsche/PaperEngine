#pragma once
#include "EditorResources.h"
#include "Engine.h"
#include "asset/manager/AssetManager.h"
#include "imgui/ImGuiLayer.h"
#include "imgui/ImGuiUtils.h"
#include "renderer/Font.h"
#include "utils/Utils.h"

namespace Paper::UI
{

	inline float GetAvailableContentSpace()
	{
		return ImGui::GetContentRegionAvail().x - 10;
	}

	inline bool BeginDropdown(const std::string& label, const std::string& preview)
	{
		const bool modified = ImGui::BeginCombo(label.c_str(), preview.c_str());
		UI::DrawItemActivityOutline();
		return modified;
	}

	inline void EndDropdown()
	{
		ImGui::EndCombo();
	}

	inline bool Selectable(const std::string& value, bool isSelected)
	{
		bool modified = ImGui::Selectable(value.c_str(), isSelected);

		//UI::DrawItemActivityOutline();
		return modified;
	}

	//BUTTONS

	inline void ImageEffects(const Shr<Texture>& image, ImRect imageArea, ImVec4 tintNormal, ImVec4 tintHovered, ImVec4 tintPressed, ImRect effectArea = ImRect())
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		if (effectArea.GetWidth() == 0.0f)
		{
			effectArea.Min.x = imageArea.Min.x;
			effectArea.Max.x = imageArea.Max.x;
		}
		if (effectArea.GetHeight() == 0.0f)
		{
			effectArea.Min.y = imageArea.Min.y;
			effectArea.Max.y = imageArea.Max.y;
		}

		ImVec4 tint = tintNormal;

		if (effectArea.Contains(ImGui::GetMousePos()))
		{
			tint = tintHovered;
			if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
				tint = tintPressed;
		}


		drawList->AddImage((void*)image->GetID(), imageArea.Min, imageArea.Max, ImVec2(0, 1), ImVec2(1, 0), ImGui::ColorConvertFloat4ToU32(tint));
	}

	inline bool Button(const std::string& text, ImVec2 size = ImVec2(0, 0))
	{
		const bool modified = ImGui::Button(text.c_str(), size);
		UI::DrawItemActivityOutline();
		return modified;
	}

	inline bool ImageButton(const Shr<Texture>& texture, ImVec2 size = ImVec2(0, 0))
	{
		CORE_ASSERT(texture, "");
		const bool modified = ImGui::ImageButton((void*)texture->GetID(), size, {0, 1}, {1, 0});
		UI::DrawItemActivityOutline();
		return modified;
	}

	inline void Image(const Shr<Texture>& texture, ImVec2 size = ImVec2(0, 0))
	{
		CORE_ASSERT(texture, "");
		ImGui::Image((void*)texture->GetID(), size, { 0, 1 }, { 1, 0 });
		UI::DrawItemActivityOutline();
	}

	inline bool CheckBox(const std::string& name, bool& val)
	{
		const bool modified = ImGui::Checkbox(name.c_str(), &val);
		UI::DrawItemActivityOutline();
		return modified;
	}

	template <typename T>
	struct ControlSettings
	{
		T speed = T(1.0f);
		T min = T();
		T max = T();
		std::string format = "";
		ImGuiSliderFlags flags = ImGuiSliderFlags_None;
	};

	template <>
	struct ControlSettings<bool>
	{
		
	};

	template <>
	struct ControlSettings<std::string>
	{
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_None;
	};

	

	inline bool FloatControl(std::string name, float& val, ControlSettings<float> settings = {})
	{
		if (settings.format.empty())
			settings.format = "%.1f";

		const bool modified = ImGui::DragFloat(name.c_str(), &val, settings.speed, settings.min, settings.max, settings.format.c_str(), settings.flags);
		UI::DrawItemActivityOutline();
		return modified;
	}

	inline bool IntControl(std::string name, int& val, ControlSettings<int> settings = {1, 0, 0, "%d"})
	{
		if (settings.format.empty())
			settings.format = "%d";
		const bool modified = ImGui::DragInt(name.c_str(), &val, settings.speed, settings.min, settings.max, settings.format.c_str(), settings.flags);
		UI::DrawItemActivityOutline();
		return modified;
	}

	inline bool StringControl(std::string name, std::string& val, ControlSettings<std::string> settings = ControlSettings<std::string>())
	{
		const bool modified = ImGui::InputText(name.c_str(), &val, settings.flags);
		UI::DrawItemActivityOutline();
		return modified;
	}

	inline bool StringControlRaw(std::string name, char* val, size_t size, ControlSettings<std::string> settings)
	{
		const bool modified = ImGui::InputText(name.c_str(), val, size, settings.flags);
		UI::DrawItemActivityOutline();
		return modified;
	}

	inline bool SearchWidget(std::string& searchString, const char* hint = "Search...", bool* grabFocus = nullptr)
	{
		PushID();

		ShiftCursorY(1.0f);

		bool modified = false;
		bool searching = false;

		const float areaPosX = ImGui::GetCursorPosX();
		const float framePaddingY = ImGui::GetStyle().FramePadding.y;

		UI::ScopedStyle rounding(ImGuiStyleVar_FrameRounding, 3.0f);
		UI::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(28.0f, framePaddingY));

		std::string searchBuffer = searchString;
		if (ImGui::InputText(GenerateID(), &searchBuffer))
		{
			searchString = searchBuffer;
			modified = true;
		}
		else if (ImGui::IsItemDeactivatedAfterEdit())
		{
			searchString = searchBuffer;
			modified = true;
		}

		const float inputLength = ImGui::GetItemRectSize().x;

		searching = !searchBuffer.empty();
		

		if (grabFocus && *grabFocus)
		{
			if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)
				&& !ImGui::IsAnyItemActive()
				&& !ImGui::IsMouseClicked(0))
			{
				ImGui::SetKeyboardFocusHere(-1);
			}

			if (ImGui::IsItemFocused())
				*grabFocus = false;
		}

		UI::DrawItemActivityOutline();
		ImGui::SetItemAllowOverlap();

		ImGui::SameLine(areaPosX + 5.0f);

		//if (layoutSuspended)
		//	ImGui::ResumeLayout();
		//
		//ImGui::BeginHorizontal(GenerateID(), ImGui::GetItemRectSize());
		const ImVec2 iconSize(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight());

		// Search icon
		{
			const float iconYOffset = framePaddingY + 1;// -3.0f;
			UI::ShiftCursorY(iconYOffset);
			ImGui::Image((void*)EditorResources::SearchIcon->GetID(), iconSize, ImVec2(0, 1), ImVec2(1, 0));
			UI::ShiftCursorY(-iconYOffset);

			// Hint
			if (!searching)
			{
				UI::ShiftCursorY(-framePaddingY);
				UI::ScopedColour text(ImGuiCol_Text, Colors::Theme::textDarker);
				UI::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(0.0f, framePaddingY));
				ImGui::SameLine();
				ImGui::TextUnformatted(hint);
				UI::ShiftCursorY(-1.0f);
			}
		}

		UI::ShiftCursorY(-1.0f);
		UI::PopID();
		return modified;
	}

	inline bool MultilineTextInput(std::string name, std::string& val, ImVec2 size, ImGuiInputTextFlags flags)
	{
		bool modified = ImGui::InputTextMultiline(name.c_str(), &val, size, flags);
		UI::DrawItemActivityOutline();
		return modified;
	}

	inline bool ColorPicker3(std::string name, glm::vec3& color)
	{
		bool modified = ImGui::ColorPicker3(name.c_str(), &color.x, ImGuiColorEditFlags_AlphaPreviewHalf);
		UI::DrawItemActivityOutline();
		return modified;
	}

	inline bool ColorPicker4(std::string name, glm::vec4& color)
	{
		bool modified = ImGui::ColorPicker4(name.c_str(), & color.x, ImGuiColorEditFlags_AlphaPreviewHalf);
		UI::DrawItemActivityOutline();
		return modified;
	}

	

	//PROPERTIES

	inline void BeginPropertyGrid()
	{
		PushID();
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::BeginTable(GenerateID(), 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable); //
		ImGui::TableSetupColumn("name"); // ImGui::GetContentRegionAvail().x / 3
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
		Shr<Texture> gearTexture = EditorResources::GearIcon;

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

	using PropertyStatusField = uint32_t;

	enum PropertyStatus
	{
		ROW_HOVERED = BIT(0),
		ROW_CLICKED_LEFT = BIT(1),
		ROW_CLICKED_MIDDLE = BIT(2),
		ROW_CLICKED_RIGHT = BIT(3),
		//ROW_DOWN = BIT(4)
	};

	inline PropertyStatusField EndPropertyElementInternal()
	{
		PropertyStatusField status = 0;

		ImGui::PopStyleVar();
		UI::ShiftCursorY(6);

		ImGui::PopID();

		const int rowHeight = 40.0f;

		const ImVec2 rowAreaMin = ImGui::TableGetCellBgRect(ImGui::GetCurrentTable(), 0).Min;
		const ImVec2 rowAreaMax = { ImGui::TableGetCellBgRect(ImGui::GetCurrentTable(), ImGui::TableGetColumnCount() - 1).Max.x - 20,
									rowAreaMin.y + rowHeight };

		//ImGui::Text(fmt::format("RowAreaMin {0}, {1}", rowAreaMin.x, rowAreaMin.y).c_str());
		//ImGui::Text(fmt::format("RowAreaMax {0}, {1}", rowAreaMax.x, rowAreaMax.y).c_str());
		//ImGui::Text(fmt::format("CurserPos {0}, {1}", ImGui::GetMousePos().x, ImGui::GetMousePos().y).c_str());

		//ImGui::PushClipRect(rowAreaMin, rowAreaMax, false);
		//bool isRowHovered, held;
		//bool isRowClicked = ImGui::ButtonBehavior(ImRect(rowAreaMin, rowAreaMax), ImGui::GetID(UI::GenerateID()),
		//	&isRowHovered, &held, ImGuiButtonFlags_AllowItemOverlap | ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

		if (ImGui::GetMousePos().x >= rowAreaMin.x && ImGui::GetMousePos().y >= rowAreaMin.y 
			&& ImGui::GetMousePos().x <= rowAreaMax.x && ImGui::GetMousePos().y <= rowAreaMax.y)
		{
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				status |= ROW_CLICKED_LEFT;
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle))
				status |= ROW_CLICKED_MIDDLE;
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
				status |= ROW_CLICKED_RIGHT;

			if (status == 0)
				status |= ROW_HOVERED;
		}

		//ImGui::PopClipRect();

		ImGui::TableSetColumnIndex(0);

		return status;
	}

	template <typename TUnderlying, typename T>
	inline ControlSettings<TUnderlying> GetControlSetting(ControlSettings<T> setting, int slot)
	{
		return { setting.speed[slot], setting.min[slot], setting.max[slot], setting.format, setting.flags };
	}
	
	inline bool PropertyInternal(bool& val, ControlSettings<bool> settings = {})
	{

		const bool modified = CheckBox("", val);

		return modified;
	}

	inline bool PropertyInternal(float& val, ControlSettings<float> settings = {})
	{
		ImGui::SetNextItemWidth(GetAvailableContentSpace());
		bool modified = FloatControl("", val, settings);

		return modified;
	}

	inline bool PropertyInternal(glm::vec2& val, ControlSettings<glm::vec2> settings = {})
	{
		constexpr int c = 2;


		float wigetWidth = GetAvailableContentSpace() / c - 2;

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
			modified |= FloatControl("", val[i], GetControlSetting<float, glm::vec2>(settings, i));
		}

		return modified;
	}

	inline bool PropertyInternal(glm::vec3& val, ControlSettings<glm::vec3> settings = {})
	{
		constexpr int c = 3;


		float wigetWidth = GetAvailableContentSpace() / c - 2;

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
			modified |= FloatControl("", val[i], GetControlSetting<float, glm::vec3>(settings, i));
		}


		return modified;
	}

	inline bool PropertyInternal(glm::vec4& val, ControlSettings<glm::vec4> settings = {})
	{
		constexpr int c = 4;


		float wigetWidth = GetAvailableContentSpace() / c - 3;

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
			modified |= FloatControl("", val[i], GetControlSetting<float, glm::vec4>(settings, i));
		}


		return modified;
	}

	inline bool PropertyInternal(int& val, ControlSettings<int> settings = {})
	{

		ImGui::SetNextItemWidth(GetAvailableContentSpace());
		bool modified = IntControl("", val, settings);


		return modified;
	}

	inline bool PropertyInternal(glm::ivec2& val, ControlSettings<glm::ivec2> settings = {})
	{
		constexpr int c = 2;


		float wigetWidth = GetAvailableContentSpace() / c - 2;

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
			modified |= IntControl("", val[i], GetControlSetting<int, glm::ivec2>(settings, i));
		}


		return modified;
	}

	inline bool PropertyInternal(glm::ivec3& val, ControlSettings<glm::ivec3> settings = {})
	{
		constexpr int c = 3;

		float wigetWidth = GetAvailableContentSpace() / c - 2;

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
			modified |= IntControl("", val[i], GetControlSetting<int, glm::ivec3>(settings, i));
		}


		return modified;
	}

	inline bool PropertyInternal(glm::ivec4& val, ControlSettings<glm::ivec4> settings = {})
	{
		constexpr int c = 4;


		float wigetWidth = GetAvailableContentSpace() / c - 3;

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
			modified |= IntControl("", val[i], GetControlSetting<int, glm::ivec4>(settings, i));
		}


		return modified;
	}

	inline bool PropertyInternal(std::string& val, ControlSettings<std::string> settings = {})
	{
		float wigetWidth = GetAvailableContentSpace();

		ImGui::SetNextItemWidth(wigetWidth);
		bool modified = StringControl("", val, settings);

		return modified;
	}

	template <typename T>
	inline bool Property(const std::string& label, T& val, T defaultVal = T(), ControlSettings<T> settings = {})
	{
		UI::BeginPropertyElementInternal(label);

		bool modified = PropertyInternal(val, settings);

		if ((UI::EndPropertyElementInternal() & ROW_CLICKED_RIGHT) && ImGui::IsKeyDown(ImGuiKey_LeftShift))
			val = defaultVal;

		return modified;
	}

	inline bool PropertyInputRaw(const std::string& label, char* val, size_t size, ControlSettings<std::string> settings = {})
	{
		UI::BeginPropertyElementInternal(label);

		float wigetWidth = GetAvailableContentSpace();

		ImGui::SetNextItemWidth(wigetWidth);
		bool modified = StringControlRaw("", val, size, settings);

		UI::EndPropertyElementInternal();

		return modified;
	}

	inline bool PropertyMultiline(const std::string& label, std::string& val, ImVec2 size = ImVec2(0, 0), ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
	{
		UI::BeginPropertyElementInternal(label);
		bool modified = UI::MultilineTextInput("", val, size, flags | ImGuiInputTextFlags_Multiline);

		UI::EndPropertyElementInternal();
		return modified;
	}

	inline bool PropertyMultiline(const std::string& label, std::string& val, float height, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
	{
		UI::BeginPropertyElementInternal(label);

		ImVec2 size;
		size.x = GetAvailableContentSpace();
		size.y = height;

		bool modified = UI::MultilineTextInput("", val, size, flags | ImGuiInputTextFlags_Multiline);

		UI::EndPropertyElementInternal();
		return modified;
	}
	

	template <typename TEnum, typename TUnderlyingType = uint32_t>
	inline bool PropertyDropdown(const std::string& label, std::vector<std::string> list, TEnum& selected)
	{
		UI::BeginPropertyElementInternal(label);

		TUnderlyingType index = (TUnderlyingType)selected;
		std::string current = list[index];

		bool modified = false;


		ImGui::SetNextItemWidth(GetAvailableContentSpace());

		if (UI::BeginDropdown(fmt::format("##{}", label), current))
		{
			for (int i = 0; i < list.size(); i++)
			{
				bool isSelected = current == list[i];
				if(UI::Selectable(list[i].c_str(), isSelected))
				{
					selected = (TEnum)i;
					modified = true;
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			UI::EndDropdown();
		}
		UI::EndPropertyElementInternal();

		return modified;
	}

	inline bool PropertyTexture(const std::string& label, const AssetHandle& textureHandle, ImVec2 size)
	{
		UI::BeginPropertyElementInternal(label);

		bool modified = false;

		Shr<Texture> texture = AssetManager::GetAsset<Texture>(textureHandle);
		if (texture)
			modified = UI::ImageButton(texture, size);
		else
			modified = UI::Button("null", size);

		UI::EndPropertyElementInternal();

		return modified;
	}

	inline bool PropertyTexture(const std::string& label, const AssetHandle& textureHandle, float maxHeight)
	{
		UI::BeginPropertyElementInternal(label);

		bool modified = false;

		Shr<Texture> texture = AssetManager::GetAsset<Texture>(textureHandle);
		if (texture)
		{
			const TextureSpecification& spec = texture->GetSpecification();

			ImVec2 size;

			size.x = GetAvailableContentSpace();
			size.y = GetAvailableContentSpace() / spec.GetRatio();
			if (size.y > maxHeight)
			{
				size.y = maxHeight;
				size.x = size.y * spec.GetRatio();
			}

			modified = UI::ImageButton(texture, size);
		}
		else
		{
			if (textureHandle)
				modified = UI::Button(textureHandle.toString());
			else
				modified = UI::Button("null");
		}

		UI::EndPropertyElementInternal();

		return modified;
	}

	inline bool PropertyFont(const std::string& label, Ref<Font>& font)
	{
		UI::BeginPropertyElementInternal(label);

		ImGui::SetNextItemWidth(GetAvailableContentSpace());

		std::string buttonText = "";

		if (font)
			buttonText = font->GetFontName();

		bool modified = UI::Button(buttonText);

		UI::EndPropertyElementInternal();

		return modified;
	}

	inline const ImGuiPayload* DragDropTargetInternal(const char* type)
	{
		const ImGuiPayload* payload = nullptr;
		if (ImGui::BeginDragDropTarget())
		{
			payload = ImGui::AcceptDragDropPayload(type);
			
			ImGui::EndDragDropTarget();
		}
		return payload;
	}

	inline std::vector<AssetHandle> DragDropTargetAssetPrev()
	{
		std::vector<AssetHandle> assetHandles;
		const ImGuiPayload* payloadPreview = ImGui::AcceptDragDropPayload("asset_payload", ImGuiDragDropFlags_AcceptPeekOnly);
		const size_t handleCount = payloadPreview->DataSize / sizeof(AssetHandle);
		const AssetHandle* handlePayload = (AssetHandle*)payloadPreview->Data;
		for (size_t i = 0; i < handleCount; i++)
		{
			assetHandles.push_back(handlePayload[i]);
		}

		return assetHandles;
	}

	inline bool DragDropTarget(Shr<Texture>& texture)
	{
		bool isRightDelivery = false;
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payloadPreview = ImGui::AcceptDragDropPayload("asset_payload", ImGuiDragDropFlags_AcceptPeekOnly);
			if (payloadPreview->DataSize / sizeof(PaperID) == 1)
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("asset_payload"))
				{

				}
			}
			

			ImGui::EndDragDropTarget();
		}
		//if (const ImGuiPayload* payload = DragDropTargetInternal("TEXTURE"))
		//{
		//	const wchar_t* path = (const wchar_t*)payload->Data;
		//	std::string file = std::filesystem::path(path).string();
		//
		//	texture = DataPool::GetAssetTexture(file, true);
		//	return true;
		//}
		return false;
	}

	inline bool DragDropTarget(Ref<Font>& font)
	{
		if (const ImGuiPayload* payload = DragDropTargetInternal("FONT"))
		{
			const wchar_t* path = (const wchar_t*)payload->Data;
			std::string file = std::filesystem::path(path).string();

			font = DataPool::GetFont(file, true);
			return true;
		}
		return false;
	}

	inline bool DragDropTarget(PaperID& paperID, AssetType type)
	{
		if (ImGui::BeginDragDropTarget())
		{
			const auto assetHandles = DragDropTargetAssetPrev();

			if (assetHandles.size() == 1 && AssetManager::GetAssetType(assetHandles[0]) == type)
			{
				if (ImGui::AcceptDragDropPayload("asset_payload"))
					paperID = assetHandles[0];
			}


			ImGui::EndDragDropTarget();
		}

		return false;
	}

	inline bool PropertyColor(const std::string& label, glm::vec3& color)
	{
		UI::BeginPropertyElementInternal(label);

		bool modified = UI::ColorPicker3("", color);

		UI::EndPropertyElementInternal();
		return modified;
	}

	inline bool PropertyColor(const std::string& label, glm::vec4& color)
	{
		UI::BeginPropertyElementInternal(label);

		bool modified = UI::ColorPicker4("", color);

		UI::EndPropertyElementInternal();
		return modified;
	}

	inline bool PropertyButton(const std::string& label, const std::string& text)
	{
		UI::BeginPropertyElementInternal(label);


		bool modified = UI::Button(text, ImVec2(GetAvailableContentSpace(), 0));

		UI::EndPropertyElementInternal();

		return modified;
	}

	inline bool PropertyPaperID(const std::string& label, PaperID& paperID)
	{
		UI::BeginPropertyElementInternal(label);
		
		ImGui::SetNextItemWidth(GetAvailableContentSpace());

		std::string buttonText = paperID.toString();
		if (paperID.ToEntity())
			buttonText = paperID.ToEntity().GetName();

		bool modified = UI::Button(buttonText);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && ImGui::IsKeyDown(ImGuiKey_LeftShift))
		{
			paperID = 0;
		}


		UI::EndPropertyElementInternal();

		return modified;
	}

	
	
}
