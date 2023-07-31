#pragma once
#include "Engine.h"

#include "Camera.h"

namespace Paper
{

	class EditorCamera : public Camera
	{
	public:
		EditorCamera();
		~EditorCamera() override = default;

		void ResetValues();

		glm::mat4 GetViewMatrix() const;

		glm::vec3 GetFront() const;

		void Calculate() override;

		void ControlCamera(float x, float y, bool state);

		glm::vec3 position{};
		float aspect_ratio;

		glm::vec3 target{};

		float pitch;
		float yaw;
		float roll;

		float fov;

		float last_x, last_y;

		

		float left_frostum{};
		float right_frostum{};
		float bottom_frostum{};
		float top_frostum{};

		float near_plane{};
		float far_plane{};

	private:
		glm::mat4 cameraViewMatrix{};

		glm::vec3 direction{};
	};

}