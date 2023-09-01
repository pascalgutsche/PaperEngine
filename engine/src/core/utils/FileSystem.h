#pragma once

namespace Paper
{
	class FileSystem
	{
	public:

		static bool Exists(const std::filesystem::path& filePath);
		static bool Move(const std::filesystem::path& oldFilePath, const std::filesystem::path& newFilePath);
		static bool Rename(const std::filesystem::path& oldFilePath, const std::filesystem::path& newFilePath);

		struct Filter
		{
			const char* name;
			const char* spec;
		};

		static std::filesystem::path OpenFile(const std::filesystem::path& defaultPath = "", const std::initializer_list<Filter>& filter = {});
		static std::filesystem::path OpenFolder(const std::filesystem::path& defaultPath = "");

		static std::filesystem::path SaveFile(const std::filesystem::path& defaultPath = "", const std::initializer_list<Filter>& filter = {});
	};
}

