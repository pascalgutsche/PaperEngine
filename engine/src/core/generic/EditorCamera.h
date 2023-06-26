#pragma once
#include "Engine.h"

namespace ppr
{
	class EditorCamera
	{
	public:
		EditorCamera();
		~EditorCamera() = default;

		void ResetValues();

		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix() const;
		glm::mat4 GetOrthographicMatrix() const;

		glm::vec3 GetFront() const;

		void Update();

	private:
		glm::vec3 target{};
		glm::vec3 position{};

		float pitch;
		float yaw;
		float roll;

		float fov;
		float aspect_ratio;

		float last_x, last_y;

		// X
		glm::vec3 cameraX{};
		// Y
		glm::vec3 cameraY{};
		// Z
		glm::vec3 cameraZ{};

		glm::mat4 cameraViewMatrix{};

		glm::vec3 front{};

		glm::vec3 directionA{};
		glm::vec3 directionB{};

		float left_frostum{};
		float right_frostum{};
		float bottom_frostum{};
		float top_frostum{};

		float near_plane{};
		float far_plane{};

	};

}