#pragma once
#include "Engine.h"

#include "utils/FileDialog.h"

namespace engine
{
	class GLFWFileDialog : public FileDialog
	{
	public:
		GLFWFileDialog();
		~GLFWFileDialog() override;

		std::string OpenFile(std::string filter) override;
		std::string SaveFile(std::string filter) override;
	};
}

