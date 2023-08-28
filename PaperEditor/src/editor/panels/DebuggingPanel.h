#pragma once
#include "EditorPanel.h"

class PaperLayer;

namespace PaperED
{
	class ApplicationPanel : public EditorPanel
	{
	public:
		void OnImGuiRender(bool& isOpen) override;
	};

	class ViewportDebuggingPanel : public EditorPanel
	{
	public:
		ViewportDebuggingPanel(PaperLayer& paperLayer);

		void OnImGuiRender(bool& isOpen) override;

	private:
		PaperLayer& paperLayer;
	};

	class SceneDebuggingPanel : public EditorPanel
	{
	public:
		void OnImGuiRender(bool& isOpen) override;

	};
}

