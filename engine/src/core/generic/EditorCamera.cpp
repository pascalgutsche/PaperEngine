#include "Engine.h"
#include "EditorCamera.h"

#include "Application.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace ppr
{
	struct DefaultValues
	{
		// GENERIC
		inline static glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f);

		inline static float pitch = 0.0f;
		inline static float yaw = 0.0f;
		inline static float roll = 0.0f;

		inline static glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);

		inline static float fov = 45.0f;
		inline static float near_plane = 0.0f;
		inline static float far_plane = 100.0f;

		// ORTHOGRAPHIC
		inline static float left_frostum = -1.0f;
		inline static float right_frostum = 1.0f;
		inline static float bottom_frostum = -1.0f;
		inline static float top_frostum = 1.0f;
	};

	EditorCamera::EditorCamera()
	{
		ResetValues();

		last_x = (float)Application::GetWindow()->GetWidth() / 2.0f;
		last_y = (float)Application::GetWindow()->GetHeight() / 2.0f;

		aspect_ratio = (float)Application::GetWindow()->GetWidth() / (float)Application::GetWindow()->GetHeight();
	}

	void EditorCamera::ResetValues()
	{
		position = DefaultValues::position;

		pitch = DefaultValues::pitch;
		yaw = DefaultValues::yaw;
		roll = DefaultValues::roll;

		//front = DefaultValues::front;

		fov = DefaultValues::fov;
		near_plane = DefaultValues::near_plane;
		far_plane = DefaultValues::far_plane;

		left_frostum = DefaultValues::left_frostum;
		right_frostum = DefaultValues::right_frostum;
		bottom_frostum = DefaultValues::bottom_frostum;
		top_frostum = DefaultValues::top_frostum;
	}

	glm::mat4 EditorCamera::GetViewMatrix() const
	{
		return cameraViewMatrix;
	}

	glm::mat4 EditorCamera::GetProjectionMatrix() const
	{
		return glm::perspective(glm::radians(fov), aspect_ratio, near_plane, far_plane);
	}

	glm::mat4 EditorCamera::GetOrthographicMatrix() const
	{
		return glm::ortho(left_frostum, right_frostum, bottom_frostum, top_frostum, near_plane, far_plane);
	}

	glm::vec3 EditorCamera::GetFront() const
	{
		return glm::normalize(directionB);
	}

	void EditorCamera::Update()
	{
		this->target = glm::vec3(this->position.x, this->position.y, 0.0f);

		//directionA.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
		//directionA.y = glm::sin(glm::radians(pitch));
		//directionA.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

		directionB.y = glm::sin(glm::radians(pitch));
		directionB.x = glm::cos(glm::radians(yaw));
		directionB.z = glm::sin(glm::radians(yaw));

		//front = glm::normalize(directionA);

		//// this is being used for pointing upwards (y globally, e.g. in the world)
		//constexpr auto up = glm::vec3(0.0f, 1.0f, 0.0f);
		//// declare z-axis
		//cameraZ = glm::normalize(position - target);
		//// declare x-axis by the cross-product
		//cameraX = glm::normalize(glm::cross(up, cameraZ));
		//// declare y-axis by the cross-product of z and x
		//cameraY = glm::cross(cameraZ, cameraX);
		//// make camera depend on position (who could have thought this)
		//cameraViewMatrix = glm::lookAt(position, position + front, up);

		//

		glm::quat orientation = glm::quat(glm::vec3(glm::radians(pitch), -glm::radians(yaw), 0.0f));

		cameraViewMatrix = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
		cameraViewMatrix = glm::inverse(cameraViewMatrix);
	}

	void EditorCamera::ControlCamera(const float x, const float y, bool state)
	{
		if (state)
		{
			last_x = x;
			last_y = y;
		}

		float dX = x - last_x;
		float dY = last_y - y;

		last_x = x;
		last_y = y;

		if (Application::GetWindow()->IsCursorEnabled()) return;

		constexpr float sens = 0.1f;
		dX *= sens;
		dY *= sens;

		yaw += dX;

		while (yaw >= 360)
		{
			yaw -= 360;
		}
		while (yaw <= -360)
		{
			yaw += 360;
		}


		if (pitch + dY > 89.9f)
			pitch = 89.9f;
		else if (pitch + dY < -89.9f)
			pitch = -89.9f;
		else
			pitch += dY;
	}
}
