#pragma once

#include <ImGui/imgui.h>

#include <ImGui/imgui_internal.h>

#include "ImGuiLayer.h"

#define CONST_UI_ID ("##" + CutStringAfterLastSlash(__FILE__ + std::to_string(__LINE__))).c_str()


namespace Paper::UI
{
	typedef int OutlineFlags;
	enum OutlineFlags_
	{
		OutlineFlags_None = 0,   // draw no activity outline
		OutlineFlags_WhenHovered = 1 << 1,   // draw an outline when item is hovered
		OutlineFlags_WhenActive = 1 << 2,   // draw an outline when item is active
		OutlineFlags_WhenInactive = 1 << 3,   // draw an outline when item is inactive
		OutlineFlags_HighlightActive = 1 << 4,   // when active, the outline is in highlight colour
		OutlineFlags_NoHighlightActive = OutlineFlags_WhenHovered | OutlineFlags_WhenActive | OutlineFlags_WhenInactive,
		OutlineFlags_NoOutlineInactive = OutlineFlags_WhenHovered | OutlineFlags_WhenActive | OutlineFlags_HighlightActive,
		OutlineFlags_All = OutlineFlags_WhenHovered | OutlineFlags_WhenActive | OutlineFlags_WhenInactive | OutlineFlags_HighlightActive,
	};

	static int uiContextID = 0;
	static uint32_t counter = 0;
	static char idBuffer[16 + 2 + 1] = "##";
	static char labelIDBuffer[1024 + 1];


	

	//ID
	inline const char* GenerateID()
	{
		snprintf(idBuffer + 2, 16, "%u", counter++);
		return idBuffer;
	}

	inline const char* GenerateLabelID(std::string_view label)
	{
		*fmt::format_to_n(labelIDBuffer, std::size(labelIDBuffer), "{}##{}", label, counter++).out = 0;
		return labelIDBuffer;
	}

	inline void PushID()
	{
		ImGui::PushID(uiContextID++);
		counter = 0;
	}

	inline void PopID()
	{
		ImGui::PopID();
		uiContextID--;
	}

	// RECT
	inline ImRect GetItemRect()
	{
		return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	}

	inline ImRect RectExpanded(const ImRect& rect, float x, float y)
	{
		ImRect result = rect;
		result.Min.x -= x;
		result.Min.y -= y;
		result.Max.x += x;
		result.Max.y += y;
		return result;
	}

	inline ImRect RectOffset(const ImRect& rect, float x, float y)
	{
		ImRect result = rect;
		result.Min.x += x;
		result.Min.y += y;
		result.Max.x += x;
		result.Max.y += y;
		return result;
	}

	inline ImRect RectOffset(const ImRect& rect, ImVec2 xy)
	{
		return RectOffset(rect, xy.x, xy.y);
	}

	// ITEM
	inline void DrawItemActivityOutline(OutlineFlags flags = OutlineFlags_All, ImColor colourHighlight = Colors::Theme::highlight, float rounding = GImGui->Style.FrameRounding)
	{
		if (!ImGui::IsItemActivated() && false) 
			return;
		
		//LOG_DEBUG("IS ACTIVE");

		auto* drawList = ImGui::GetWindowDrawList();
		const ImRect rect = RectExpanded(GetItemRect(), 1.0f, 1.0f);
		if ((flags & OutlineFlags_WhenActive) && ImGui::IsItemActive())
		{
			if (flags & OutlineFlags_HighlightActive)
			{
				drawList->AddRect(rect.Min, rect.Max, colourHighlight, rounding, 0, 1.5f);
			}
			else
			{
				drawList->AddRect(rect.Min, rect.Max, ImColor(60, 60, 60), rounding, 0, 1.5f);
			}
		}
		else if ((flags & OutlineFlags_WhenHovered) && ImGui::IsItemHovered() && !ImGui::IsItemActive())
		{
			drawList->AddRect(rect.Min, rect.Max, ImColor(255, 128, 0), rounding, 0, 1.5f);
		}
		else if ((flags & OutlineFlags_WhenInactive) && !ImGui::IsItemHovered() && !ImGui::IsItemActive())
		{
			drawList->AddRect(rect.Min, rect.Max, ImColor(50, 50, 50), rounding, 0, 1.0f);
		}
	};

	//Cursor

	inline void ShiftCursorX(float distance)
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + distance);
	}

	inline void ShiftCursorY(float distance)
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + distance);
	}

	inline void ShiftCursor(float x, float y)
	{
		const ImVec2 cursor = ImGui::GetCursorPos();
		ImGui::SetCursorPos(ImVec2(cursor.x + x, cursor.y + y));
	}

	inline void SetCursorX(float posX)
	{
		ImGui::SetCursorPosX(posX);
	}

	inline void SetCursorY(float posY)
	{
		ImGui::SetCursorPosY(posY);
	}

	inline void SetCursor(ImVec2 pos)
	{
		ImGui::SetCursorPos(pos);
	}

	// STRUCT

	struct ScopedStyle
	{
		ScopedStyle(const ScopedStyle&) = delete;
		ScopedStyle& operator=(const ScopedStyle&) = delete;
		template<typename T>
		ScopedStyle(ImGuiStyleVar styleVar, T value) { ImGui::PushStyleVar(styleVar, value); }
		~ScopedStyle() { ImGui::PopStyleVar(); }
	};

	struct ScopedColour
	{
		ScopedColour(const ScopedColour&) = delete;
		ScopedColour& operator=(const ScopedColour&) = delete;
		template<typename T>
		ScopedColour(ImGuiCol colourId, T colour) { ImGui::PushStyleColor(colourId, colour); }
		~ScopedColour() { ImGui::PopStyleColor(); }
	};

	struct ScopedFont
	{
		ScopedFont(const ScopedFont&) = delete;
		ScopedFont& operator=(const ScopedFont&) = delete;
		ScopedFont(ImFont* font) { ImGui::PushFont(font); }
		~ScopedFont() { ImGui::PopFont(); }
	};

	struct ScopedID
	{
		ScopedID(const ScopedID&) = delete;
		ScopedID& operator=(const ScopedID&) = delete;
		template<typename T>
		ScopedID(T id) { ImGui::PushID(id); }
		~ScopedID() { ImGui::PopID(); }
	};

	
}

std::string CutStringAfterLastSlash(std::string val);

