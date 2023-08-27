#pragma once
#include "generic/Hash.h"
#include "panels/EditorPanel.h"

struct PanelData
{
	std::string strID = "";
	Shr<EditorPanel> panel = nullptr;
	bool isOpen = false;
	bool firstRender = true;
};

class PanelManager
{
public:
	void OnImGuiRender();
	void OnEvent(Event& e);

	std::unordered_map<uint32_t, PanelData>& GetPanels() { return panels; }

	void RemovePanel(const std::string& strID)
	{
		uint32_t id = Hash::GenerateFNVHash(strID);

		if (panels.contains(id))
		{
			LOG_CORE_ERROR("Trying to remove panel that did not exists: '{}'", strID);
			return;
		}
		panels.erase(id);
	};

	template <typename TEditorPanel, typename... TArgs>
	Shr<TEditorPanel> AddPanel(const std::string& strID, bool isOpenByDefault, TArgs&&... args)
	{
		static_assert(std::is_base_of<EditorPanel, TEditorPanel>::value, "TEditorPanel must be a derived of EditorPanel");

		uint32_t id = Hash::GenerateFNVHash(strID);

		if (panels.contains(id))
		{
			LOG_CORE_ERROR("Trying to add panel '{}' multiple times", strID);
			return nullptr;
		}

		Shr<TEditorPanel> editorPanel = MakeShr<TEditorPanel>(std::forward<TArgs>(args)...);
		panels[id] = { .strID = strID, .panel = editorPanel, .isOpen = isOpenByDefault };

		return editorPanel;
	}

private:
	std::unordered_map<uint32_t, PanelData> panels;
};
