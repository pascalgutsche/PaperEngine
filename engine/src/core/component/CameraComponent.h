#pragma once
#include "camera/EntityCamera.h"

#include "Serializable.h"

namespace Paper
{
	struct CameraComponent : public Serializable
	{
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		bool fixedAspectRatio = false;
		bool primary = true;

		EntityCamera camera;

		bool Serialize(YAML::Emitter& out) override;
		void SerializeEntityCamera(YAML::Emitter& out) const;
		bool Deserialize(YAML::Node& data) override;
		void DeserializeEntityCamera(YAML::Node& data);
	};
}
