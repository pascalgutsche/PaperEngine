#pragma once
#include "Engine.h"


#include <gtx/quaternion.hpp>

#include "serializer/Serializable.h"

namespace Paper
{
	struct TransformComponent : Serializable
	{
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;

		TransformComponent()
			: position(glm::vec3(0.0f)), scale(glm::vec3(1.0f)), rotation(glm::vec3(0.0f)) { }

		TransformComponent(glm::vec3 position)
			: position(position), scale(glm::vec3(1.0f)), rotation(glm::vec3(0.0f)) { }

		TransformComponent(glm::vec3 position, glm::vec3 scale)
			: position(position), scale(scale), rotation(glm::vec3(0.0f)) { }

		TransformComponent(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
			: position(position), scale(scale), rotation(rotation) { }

		void UpdateRotation()
		{
			rotation.x = (float)((int)(rotation.x * 100) % 36000) / 100.0f;
			rotation.y = (float)((int)(rotation.y * 100) % 36000) / 100.0f;
			rotation.z = (float)((int)(rotation.z * 100) % 36000) / 100.0f;
		}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation_mat = glm::toMat4(glm::quat(glm::radians(rotation)));

			return glm::translate(glm::mat4(1.0f), position)
				* rotation_mat
				* glm::scale(glm::mat4(1.0f), scale);
			//return glm::translate(glm::mat4(1.0f), position) *
			//	glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
			//	glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
			//	glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
			//	glm::scale(glm::mat4(1.0f), scale);
		}

		bool Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node& data) override;
	};

	
}


