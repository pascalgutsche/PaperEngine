#pragma once
#include "DockManager.h"
#include "generic/Hash.h"
#include "panels/EditorPanel.h"

class PaperLayer;

namespace PaperED
{
	enum class DockLoc;


	enum class PanelOpenSetting
	{
		View, Edit,

		None,
		_COUNT
	};

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
		void OnProjectChanged(const Shr<Project>& project);

		void RemovePanel(const std::string& strID);
		void OpenPanel(const std::string& strID);

		template <typename TEditorPanel, typename... TArgs>
		Shr<TEditorPanel> AddPanel(const PanelData& panelData)
		{
			static_assert(std::is_base_of<EditorPanel, TEditorPanel>::value, "TEditorPanel must be a derived of EditorPanel");

			auto& panelsInSetting = panels[(size_t)panelOpenSetting];

			uint32_t id = Hash::GenerateFNVHash(panelData.strID);

			if (panelsInSetting.contains(id))
			{
				LOG_CORE_ERROR("Trying to add panel '{}' multiple times", panelData.strID);
				return nullptr;
			}
			panelsInSetting[id] = panelData;
			panelsInSetting[id].panel->panelName = panelData.displayName;
			panelsInSetting[id].panel->paperLayer = paperLayer;

			return std::dynamic_pointer_cast<TEditorPanel>(panelsInSetting[id].panel);
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

		std::unordered_map<uint32_t, PanelData>& GetPanels(PanelOpenSetting setting) { return panels[(size_t)setting]; }

		static void SetPanelOpenSetting(PanelOpenSetting setting) { CORE_ASSERT(setting != PanelOpenSetting::_COUNT, ""); panelOpenSetting = setting; };

	private:
		std::array<std::unordered_map<uint32_t, PanelData>, (size_t)PanelOpenSetting::_COUNT> panels;
		PaperLayer* paperLayer = nullptr;

		inline static PanelOpenSetting panelOpenSetting = PanelOpenSetting::None;

		friend class PaperLayer;
	};
}
