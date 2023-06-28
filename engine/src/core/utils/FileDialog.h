#pragma once
#include "Engine.h"

namespace ppr
{
	class FileDialog {
	public:

	    virtual ~FileDialog() = default;

		virtual std::filesystem::path OpenFile(std::string filter) = 0;
		virtual std::filesystem::path SaveFile(std::string filter) = 0;

	    static Shr<FileDialog> Create();
	};
}