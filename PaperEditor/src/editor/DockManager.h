#pragma once

namespace PaperED
{
	enum class DockLoc
	{
		Main, Bottom, Top, Left, Right, LeftBottom, RightBottom, _COUNT, NONE
	};

	class DockManager
	{
	public:
		inline static ImGuiID dockspace_id = 0;
		//inline static ImGuiID dock_id_main = 0;
		//inline static ImGuiID dock_id_down = 0;
		//inline static ImGuiID dock_id_left = 0;
		//inline static ImGuiID dock_id_right = 0;
		//inline static ImGuiID dock_id_right_bottom = 0;
		//inline static ImGuiID dock_id_right2 = 0;
		//inline static ImGuiID dock_id_left_bottom = 0;
		//inline static ImGuiID dock_id_up = 0;

		static void CreateNodes();

		static void Update();

		static void DockPanel(const std::string& panelName, DockLoc dockLocation);

	private:
		inline static std::unordered_map<std::string, ImGuiID> dockPanelQueue;
		inline static std::array<ImGuiID, (size_t)DockLoc::_COUNT> dockIDs;
	};
}

