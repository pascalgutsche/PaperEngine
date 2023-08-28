#pragma once
#include "DockManager.h"
#include "generic/Hash.h"
#include "panels/EditorPanel.h"

namespace PaperED
{
	enum class DockLoc;


	struct PanelData
	{
		std::string strID = "";
		std::string displayName = "Editor Panel";
		Shr<EditorPanel> panel = nullptr;
		bool isOpen = false;
		bool firstRender = true;
		DockLoc initialDockLocation = DockLoc::NONE;
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
		Shr<TEditorPanel> AddPanel(const PanelData& panelData)
		{
			static_assert(std::is_base_of<EditorPanel, TEditorPanel>::value, "TEditorPanel must be a derived of EditorPanel");

			uint32_t id = Hash::GenerateFNVHash(panelData.strID);

			if (panels.contains(id))
			{
				LOG_CORE_ERROR("Trying to add panel '{}' multiple times", panelData.strID);
				return nullptr;
			}
			panels[id] = panelData;
			panels[id].panel->panelName = panelData.displayName;

			return std::dynamic_pointer_cast<TEditorPanel>(panels[id].panel);
		}

		template <typename TEditorPanel, typename... TArgs>
		Shr<TEditorPanel> AddPanel(bool isOpenByDefault, TArgs&&... args)
		{
			std::string strID = Utils::TypeToStdString<TEditorPanel>();
			return AddPanel<TEditorPanel>({ .strID = strID, .displayName = strID, .panel = MakeShr<TEditorPanel>(std::forward<TArgs>(args)...), .isOpen = isOpenByDefault });
		}

		template <typename TEditorPanel, typename... TArgs>
		Shr<TEditorPanel> AddPanel(bool isOpenByDefault, DockLoc initialDockLoc, TArgs&&... args)
		{
			std::string strID = Utils::TypeToStdString<TEditorPanel>();
			return AddPanel<TEditorPanel>({ .strID = strID, .displayName = strID, .panel = MakeShr<TEditorPanel>(std::forward<TArgs>(args)...), .isOpen = isOpenByDefault, .initialDockLocation = initialDockLoc });
		}

		template <typename TEditorPanel, typename... TArgs>
		Shr<TEditorPanel> AddPanel(const char* displayName, bool isOpenByDefault, TArgs&&... args)
		{
			std::string strID = Utils::TypeToStdString<TEditorPanel>();
			return AddPanel<TEditorPanel>({ .strID = strID, .displayName = displayName, .panel = MakeShr<TEditorPanel>(std::forward<TArgs>(args)...), .isOpen = isOpenByDefault });
		}

		template <typename TEditorPanel, typename... TArgs>
		Shr<TEditorPanel> AddPanel(const char* displayName, bool isOpenByDefault, DockLoc initialDockLoc, TArgs&&... args)
		{
			std::string strID = Utils::TypeToStdString<TEditorPanel>();
			return AddPanel<TEditorPanel>({ .strID = strID, .displayName = displayName, .panel = MakeShr<TEditorPanel>(std::forward<TArgs>(args)...), .isOpen = isOpenByDefault, .initialDockLocation = initialDockLoc });
		}

	private:
		std::unordered_map<uint32_t, PanelData> panels;
	};
}