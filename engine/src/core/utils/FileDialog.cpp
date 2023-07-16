#include "Engine.h"
#include "FileDialog.h"

#include "generic/Window.h"

#include "GLFW/GLFWFileDialog.h"

namespace Paper
{
	Shr<FileDialog> FileDialog::Create()
	{
		switch (Window::GetFramework())
		{
			case NONE: CORE_ASSERT(false, "NONE is a not valid framework"); return nullptr;
			case GLFW: return MakeShr<GLFWFileDialog>();
		}

		CORE_ASSERT(false, "ONLY WINDOWS IS CURRENTLY SUPPORTED!")
			return nullptr;
	}
}