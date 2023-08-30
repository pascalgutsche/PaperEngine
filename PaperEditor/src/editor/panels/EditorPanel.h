#pragma once

class PaperLayer;

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
		PaperLayer* paperLayer;

		friend class PanelManager;
	};
}
