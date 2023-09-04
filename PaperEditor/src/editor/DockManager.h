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

		static bool CreateNodes();

		static void Update();

		static void DockPanel(const std::string& panelName, DockLoc dockLocation);

	private:
		inline static std::unordered_map<std::string, ImGuiID> dockPanelQueue;
		inline static std::array<ImGuiID, (size_t)DockLoc::_COUNT> dockIDs;
	};
}

