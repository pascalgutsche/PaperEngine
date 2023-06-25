#pragma once
#include "Engine.h"

namespace engine
{
	struct LineComponent
	{
        glm::vec3 positionA = glm::vec3(-0.5f, 0.0f, 0.0f);
        glm::vec3 positionB = glm::vec3(0.5f, 0.0f, 0.0f);
        glm::vec4 color =  glm::vec4(1.0f);
        float thickness = 1.0f;

        LineComponent(glm::vec3 positionA, glm::vec3 positionB, glm::vec4 color, float thickness, bool registerAlphaPixelsToEvent = false);

	};
};