#pragma once

namespace Paper
{
	class FileSystem
	{
	public:

		struct Filter
		{
			const char* name;
			const char* spec;
		};

		static std::filesystem::path OpenFile(const std::filesystem::path& defaultPath = "", const std::initializer_list<std::string>& filter = {});
		static std::filesystem::path OpenFolder(const std::filesystem::path& defaultPath = "");

		static std::filesystem::path SaveFile(const std::filesystem::path& defaultPath = "", const std::initializer_list<std::string>& filter = {});
	};
}

