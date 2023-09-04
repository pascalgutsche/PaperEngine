#pragma once


namespace Paper
{
	enum class ProjectionMode
	{
		Perspective,
		Orthographic,

		_LAST = Orthographic
	};

	class Camera
	{
	public:
		Camera() = default;
		virtual ~Camera() = default;

		void SetProjectionMode(ProjectionMode _mode);
		ProjectionMode GetProjectionMode() const { return projectionMode; }

		glm::mat4 GetProjectionMatrix() const { return projectionMatrix; }

		virtual void Calculate() = 0;

	protected:
		ProjectionMode projectionMode = ProjectionMode::Perspective;

		glm::mat4 projectionMatrix;
	};

	inline std::string ProjectionModeToString(ProjectionMode mode)
	{
		switch (mode)
		{
			case ProjectionMode::Perspective: return "Perspective";
			case ProjectionMode::Orthographic: return "Orthographic";
		}
		CORE_ASSERT(false, "");
		return "";
	}

	inline ProjectionMode StringToProjectionMode(const std::string& mode)
	{
		if (mode._Equal("Perspective")) return ProjectionMode::Perspective;
		if (mode._Equal("Orthographic")) return ProjectionMode::Orthographic;

		CORE_ASSERT(false, "");
		return ProjectionMode::Perspective;
	}
}

