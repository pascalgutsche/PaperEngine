#pragma once

namespace PaperED
{
	class EditorPanel
	{
	public:
		virtual ~EditorPanel() = default;

		virtual void OnImGuiRender(bool& isOpen) = 0;
		virtual void OnEvent(Event& e) {}

	protected:
		std::string panelName;

		friend class PanelManager;
	};
}