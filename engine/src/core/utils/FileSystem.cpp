#include "Engine.h"
#include "FileSystem.h"

#include "nfd.hpp"

#include <shellapi.h>

namespace Paper
{
	bool FileSystem::Exists(const std::filesystem::path& filePath)
	{
		return std::filesystem::exists(filePath);
	}

	bool FileSystem::Move(const std::filesystem::path& oldFilePath, const std::filesystem::path& newFilePath)
	{
		return Rename(oldFilePath, newFilePath);
	}

	bool FileSystem::Rename(const std::filesystem::path& oldFilePath, const std::filesystem::path& newFilePath)
	{
		if (Exists(newFilePath))
			return false;

		std::filesystem::rename(oldFilePath, newFilePath);
		return true;
	}

	bool FileSystem::OpenExternal(const std::filesystem::path& filePath)
	{
		if (!Exists(filePath))
			return false;
		auto absolutePath = std::filesystem::canonical(filePath);
		if (!Exists(absolutePath))
			return false;

		ShellExecute(NULL, L"open", absolutePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
		return true;
	}

	bool FileSystem::ShowFileInExplorer(const std::filesystem::path& path)
	{
		auto absolutePath = std::filesystem::canonical(path);
		if (!Exists(absolutePath))
			return false;

		std::string cmd = fmt::format("explorer.exe /select,\"{0}\"", absolutePath.string());

		system(cmd.c_str());
		return true;
	}


	std::filesystem::path FileSystem::OpenFile(const std::initializer_list<FileSpecFilter>& filter, const std::filesystem::path& defaultPath)
	{
		NFD::UniquePath filePath;
		nfdresult_t result = NFD::OpenDialog(filePath, (const nfdfilteritem_t*)filter.begin(), filter.size(), defaultPath.string().c_str());

		switch (result)
		{
			case NFD_OKAY: return filePath.get();
			case NFD_CANCEL: return "";
			case NFD_ERROR:
			{
				LOG_CORE_ERROR("NFD threw an error: {}", NFD::GetError());
				return "";
			}
		}
		return "";
	}

	std::filesystem::path FileSystem::OpenFolder(const std::filesystem::path& defaultPath)
	{
		NFD::UniquePath filePath;
		nfdresult_t result = NFD::PickFolder(filePath, defaultPath.string().c_str());

		switch (result)
		{
			case NFD_OKAY: return filePath.get();
			case NFD_CANCEL: return "";
			case NFD_ERROR:
			{
				LOG_CORE_ERROR("NFD threw an error: {}", NFD::GetError());
				return "";
			}
		}
		return "";
	}

	std::filesystem::path FileSystem::SaveFile(const std::initializer_list<FileSpecFilter>& filter, const std::filesystem::path& defaultPath, const std::string& defaultName)
	{
		NFD::UniquePath filePath;
		nfdresult_t result = NFD::SaveDialog(filePath, (const nfdfilteritem_t*)filter.begin(), filter.size(), defaultPath.string().c_str(), defaultName.c_str());

		switch (result)
		{
			case NFD_OKAY: return filePath.get();
			case NFD_CANCEL: return "";
			case NFD_ERROR:
			{
				LOG_CORE_ERROR("NFD threw an error: {}", NFD::GetError());
				return "";
			}
		}
		return "";
	}
}
