#include "Engine.h"
#include "EntityCamera.h"

namespace Paper
{
	EntityCamera::EntityCamera()
	{
		Calculate();
	}

	glm::mat4 EntityCamera::GetProjectionMatrix() const
	{
		return projection;
	}

	void EntityCamera::SetPerspectiveFOV(float _perspectiveFOV)
	{
		perspectiveFOV = _perspectiveFOV;
		Calculate();
	}

	void EntityCamera::SetPerspectiveNearClip(float _nearClip)
	{
		perspectiveNear = _nearClip;
		Calculate();
	}

	void EntityCamera::SetPerspectiveFarClip(float _farClip)
	{
		perspectiveFar = _farClip;
		Calculate();
	}

	void EntityCamera::SetOrthographicSize(float _size)
	{
		orthographicSize = _size;
		Calculate();
	}

	void EntityCamera::SetOrthographicNearClip(float _nearClip)
	{
		orthographicNear = _nearClip;
		Calculate();
	}

	void EntityCamera::SetOrthographicFarClip(float _farClip)
	{
		orthographicFar = _farClip;
		Calculate();
	}

	void EntityCamera::SetProjectionMode(ProjectionMode _mode)
	{
		projectionMode = _mode;
		Calculate();
	}

	void EntityCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		CORE_ASSERT(width > 0 || height > 0, "Viewport has to be greater than 0");
		aspectRatio = (float)width / (float)height;
		Calculate();
	}

	void EntityCamera::Calculate()
	{
		if (projectionMode == ProjectionMode::Perspective)
		{
			projection = glm::perspective(glm::radians(perspectiveFOV), aspectRatio, perspectiveNear, perspectiveFar);
			return;
		}
		float orthoLeft = -orthographicSize * aspectRatio * 0.5f;
		float orthoRight = orthographicSize * aspectRatio * 0.5f;
		float orthoBottom = -orthographicSize * 0.5f;
		float orthoTop = orthographicSize * 0.5f;
		projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, orthographicNear, orthographicFar);
	}


}
