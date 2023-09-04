#include "Engine.h"
#include "Camera.h"


namespace Paper
{
	void Camera::SetProjectionMode(ProjectionMode _mode)
	{
		projectionMode = _mode;
		Calculate();
	}
}
