#include "Engine.h"
#include "FileSystem.h"

#include "nfd.hpp"

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


	std::filesystem::path FileSystem::OpenFile(const std::filesystem::path& defaultPath, const std::initializer_list<std::string>& filter)
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

	std::filesystem::path FileSystem::SaveFile(const std::filesystem::path& defaultPath, const std::initializer_list<std::string>& filter)
	{
		NFD::UniquePath filePath;
		nfdresult_t result = NFD::SaveDialog(filePath, (const nfdfilteritem_t*)filter.begin(), filter.size());

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
