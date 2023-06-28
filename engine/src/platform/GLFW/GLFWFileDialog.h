#pragma once
#include "Engine.h"

#include "utils/FileDialog.h"

namespace ppr
{
	class GLFWFileDialog : public FileDialog
	{
	public:
		GLFWFileDialog();
		~GLFWFileDialog() override;

		std::filesystem::path OpenFile(std::string filter) override;
		std::filesystem::path SaveFile(std::string filter) override;
	};
}

