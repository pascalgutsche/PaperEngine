#pragma once
#include "Engine.h"

#include "serializer/Serializable.h"

namespace Paper
{
	struct LineComponent : Serializable
	{
        glm::vec3 positionA = glm::vec3(-0.5f, 0.0f, 0.0f);
        glm::vec3 positionB = glm::vec3(0.5f, 0.0f, 0.0f);
        glm::vec4 color =  glm::vec4(1.0f);
        float thickness = 1.0f;

        LineComponent() = default;
        ~LineComponent() override = default;

        LineComponent(glm::vec3 positionA, glm::vec3 positionB, glm::vec4 color, float thickness)
	        : positionA(positionA), positionB(positionB), color(color), thickness(thickness) { }

        bool Serialize(YAML::Emitter& out) override;
        bool Deserialize(YAML::Node& data) override;
	};
};