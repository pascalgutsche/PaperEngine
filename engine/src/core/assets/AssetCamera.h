#pragma once
#include "Engine.h"

class Asset
{
	friend class YAMLSerializer;
public:
	Asset();
	~Asset();

protected:
	std::filesystem::path filePath;
	

};

