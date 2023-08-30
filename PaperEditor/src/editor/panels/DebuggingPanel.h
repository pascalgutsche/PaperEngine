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
		void OnImGuiRender(bool& isOpen) override;
	};

	class SceneDebuggingPanel : public EditorPanel
	{
	public:
		void OnImGuiRender(bool& isOpen) override;

	};

	class CameraSettingsPanel : public EditorPanel
	{
	public:
		void OnImGuiRender(bool& isOpen) override;
	};

	class ImGuiDemoPanel : public EditorPanel
	{
	public:
		void OnImGuiRender(bool& isOpen) override;
	};
}

