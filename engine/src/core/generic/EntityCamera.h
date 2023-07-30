#pragma once

namespace Paper
{
	class EntityCamera
	{
	public:
		enum class ProjectionMode
		{
			Perspective,
			Orthographic
		};
	public:
		EntityCamera();
		~EntityCamera() = default;

		glm::mat4 GetProjectionMatrix() const;

		void SetPerspectiveFOV(float _perspectiveFOV);
		float GetPerspectiveFOV() const { return perspectiveFOV; }

		void SetPerspectiveNearClip(float _nearClip);
		float GetPerspectiveNearClip() const { return perspectiveNear; }

		void SetPerspectiveFarClip(float _farClip);
		float GetPerspectiveFarClip() const { return perspectiveFar; }

		void SetOrthographicSize(float _size);
		float GetOrthographicSize() const { return orthographicSize; }

		void SetOrthographicNearClip(float _nearClip);
		float GetOrthographicNearClip() const { return orthographicNear; }

		void SetOrthographicFarClip(float _farClip);
		float GetOrthographicFarClip() const { return orthographicFar; }

		void SetProjectionMode(ProjectionMode _mode);
		ProjectionMode GetProjectionMode() const { return projectionMode; }

		void SetViewportSize(uint32_t width, uint32_t height);


		

	private:
		void Calculate();

		ProjectionMode projectionMode = ProjectionMode::Perspective;

		float perspectiveFOV = 45.0f;
		float perspectiveNear = 0.01f;
		float perspectiveFar = 1000.0f;

		float orthographicSize = 10.0f;
		float orthographicNear = -1.0f;
		float orthographicFar = 1.0f;

		float aspectRatio = 0.0f;

		glm::mat4 projection;
	};

	inline std::string ProjectionModeToString(EntityCamera::ProjectionMode mode)
	{
		switch (mode)
		{
			case EntityCamera::ProjectionMode::Perspective: return "Perspective";
			case EntityCamera::ProjectionMode::Orthographic: return "Orthographic";
		}
		CORE_ASSERT(false, "");
		return "";
	}

	inline EntityCamera::ProjectionMode StringToProjectionMode(const std::string& mode)
	{
		switch (mode)
		{
			case "Perspective": return EntityCamera::ProjectionMode::Perspective;
			case "Orthographic": return EntityCamera::ProjectionMode::Orthographic;
		}
		CORE_ASSERT(false, "");
		return EntityCamera::ProjectionMode::Perspective;
	}
}