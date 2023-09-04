#pragma once
#include "Engine.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;

	glm::vec4 color;
};

class Asset3D
{

public:
	Asset3D();
	~Asset3D();

	void OnImGui();

	//std::filesystem::path GetPath() { return filePath; }
	//std::vector<Verte

private:
	std::filesystem::path filePath;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;

	glm::vec4 color;

	bool isLightSource;
};
