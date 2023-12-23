#include "Editor.h"

#include "SelectionManager.h"

namespace PaperED
{
	void SelectionManager::Select(SelectionManagerType type, PaperID paperID)
	{
		auto& vec = selectionMap[type];
		if (std::ranges::find(vec, paperID) != vec.end())
		{
			LOG_WARN("Selecting something which is already in selectionList. '{}'", paperID.toString());
			return;
		}
		vec.push_back(paperID);
	}

	void SelectionManager::Deselect(SelectionManagerType type, PaperID paperID)
	{
		auto& vec = selectionMap[type];
		const auto it = std::ranges::find(vec, paperID);
		if (it == vec.end())
		{
			LOG_WARN("Deselecting something which is not in selectionList. '{}'", paperID.toString());
			return;
		}
		vec.erase(it);
	}

	void SelectionManager::DeselectAll(SelectionManagerType type)
	{
		selectionMap[type].clear();
	}

	void SelectionManager::DeselectAll()
	{
		selectionMap.clear();
	}

	size_t SelectionManager::GetSelectionCount(SelectionManagerType type)
	{
		const auto& vec = selectionMap[type];
		return vec.size();
	}

	std::vector<PaperID> SelectionManager::GetSelections(SelectionManagerType type)
	{
		return selectionMap[type];
	}

	bool SelectionManager::HasSelection(SelectionManagerType type)
	{
		const auto& vec = selectionMap[type];
		return !vec.empty();
	}

	bool SelectionManager::HasSelection()
	{
		return !selectionMap.empty();
	}

	bool SelectionManager::IsSelected(SelectionManagerType type, PaperID paperID)
	{
		auto& vec = selectionMap[type];
		if (std::ranges::find(vec, paperID) != vec.end())
			return true;
		return false;
	}

	
}
