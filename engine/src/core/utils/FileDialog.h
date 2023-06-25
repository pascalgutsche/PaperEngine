#pragma once
#include "Engine.h"

namespace ppr
{
	class FileDialog {
	public:

	    virtual ~FileDialog() = default;

		virtual std::string OpenFile(std::string filter) = 0;
		virtual std::string SaveFile(std::string filter) = 0;

	    static Shr<FileDialog> Create();
	};
}