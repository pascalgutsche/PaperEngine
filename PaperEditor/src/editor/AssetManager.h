#pragma once
#include "Editor.h"

class AssetManager
{
public:
	AssetManager(std::filesystem::path rootPath);
	~AssetManager() = default;

private:
	std::filesystem::path rootPath;

	std::unordered_map<std::string, std::filesystem::path> pathMap;
	std::vector<std::filesystem::path> pathHistory;
};