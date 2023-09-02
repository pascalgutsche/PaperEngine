#pragma once
#include "project/Project.h"


class PaperLayer;

namespace PaperED
{
	class EditorPanel
	{
	public:
		virtual ~EditorPanel() = default;

		virtual void OnImGuiRender(bool& isOpen) = 0;
		virtual void OnEvent(Event& e) {}
		virtual void OnProjectChanged(const Shr<Project>& project) {}
		virtual void OnSceneChanged(const Shr<Scene>& scene) {}

	protected:
		std::string panelName;
		PaperLayer* paperLayer;

		friend class PanelManager;
	};
}
