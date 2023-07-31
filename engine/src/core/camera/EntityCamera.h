#pragma once
#include "Camera.h"

namespace Paper
{
	class EntityCamera : public Camera
	{
	public:
		EntityCamera();
		~EntityCamera() override = default;

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

		void SetViewportSize(uint32_t width, uint32_t height);

	private:
		void Calculate() override;

		float perspectiveFOV = 45.0f;
		float perspectiveNear = 0.01f;
		float perspectiveFar = 1000.0f;

		float orthographicSize = 10.0f;
		float orthographicNear = -1.0f;
		float orthographicFar = 1000.0f;

		float aspectRatio = 0.0f;

	};

	
}