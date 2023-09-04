#include "Editor.h"

#include "SelectionManager.h"

namespace PaperED
{
	void SelectionManager::Select(PaperID paperID)
	{
		selection = paperID;
	}

	void SelectionManager::Deselect()
	{
		selection = 0;
	}

	PaperID SelectionManager::GetSelection()
	{
		return selection;
	}

	bool SelectionManager::HasSelection()
	{
		return !selection.Empty();
	}
}
