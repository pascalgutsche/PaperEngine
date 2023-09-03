#pragma once

namespace Paper
{
	struct FileSpecFilter
	{
		const char* name;
		const char* spec;
	};

	class FileSystem
	{
	public:

		static bool Exists(const std::filesystem::path& filePath);
		static bool Move(const std::filesystem::path& oldFilePath, const std::filesystem::path& newFilePath);
		static bool Rename(const std::filesystem::path& oldFilePath, const std::filesystem::path& newFilePath);

		

		static std::filesystem::path OpenFile(const std::initializer_list<FileSpecFilter>& filter = {}, const std::filesystem::path& defaultPath = "");
		static std::filesystem::path OpenFolder(const std::filesystem::path& defaultPath = "");

		static std::filesystem::path SaveFile(const std::initializer_list<FileSpecFilter>& filter = {}, const std::filesystem::path& defaultPath = "", const std::string& defaultName = "");
	};
}

