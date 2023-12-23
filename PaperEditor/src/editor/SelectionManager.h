#pragma once

namespace PaperED
{
	enum class SelectionManagerType
	{
		ViewPort,
		ContentBrowser
	};

	class SelectionManager
	{
	public:
		static void Select(SelectionManagerType type, PaperID paperID);
		static void Deselect(SelectionManagerType type, PaperID paperID);
		static void DeselectAll(SelectionManagerType type);
		static void DeselectAll();

		static size_t GetSelectionCount(SelectionManagerType type);

		static std::vector<PaperID> GetSelections(SelectionManagerType type);

		static bool HasSelection();
		static bool HasSelection(SelectionManagerType type);
		static bool IsSelected(SelectionManagerType type, PaperID paperID);

	private:
		inline static std::unordered_map<SelectionManagerType, std::vector<PaperID>> selectionMap;
	};
}