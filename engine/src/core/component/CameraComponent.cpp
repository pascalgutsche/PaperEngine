#include "Engine.h"
#include "CameraComponent.h"

namespace Paper
{
	bool CameraComponent::Serialize(YAML::Emitter& out)
	{
		try
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			out << YAML::Key << "FixedAspectRatio" << YAML::Value << fixedAspectRatio;
			out << YAML::Key << "Primary" << YAML::Value << primary;

			SerializeEntityCamera(out);

			out << YAML::EndMap; // CameraComponent
			return true;
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[CameraComponent]: Could not serialize component\n\t" + e.msg);
			return false;
		}
	}

	void CameraComponent::SerializeEntityCamera(YAML::Emitter& out) const
	{
		out << YAML::Key << "EntityCamera";
		out << YAML::BeginMap; // EntityCamera

		out << YAML::Key << "ProjectionMode" << YAML::Value << ProjectionModeToString(camera.GetProjectionMode());

		out << YAML::Key << "PerspectiveFOV" << camera.GetPerspectiveFOV();
		out << YAML::Key << "PerspectiveNear" << camera.GetPerspectiveNearClip();
		out << YAML::Key << "PerspectiveFar" << camera.GetPerspectiveFarClip();
		out << YAML::Key << "OrthographicSize" << camera.GetOrthographicSize();
		out << YAML::Key << "OrthographicNear" << camera.GetOrthographicNearClip();
		out << YAML::Key << "OrthographicFar" << camera.GetOrthographicFarClip();

		out << YAML::EndMap; // EntityCamera
	}

	bool CameraComponent::Deserialize(YAML::Node& data)
	{
		try
		{
			fixedAspectRatio = data["FixedAspectRatio"].as<float>();
			primary = data["Primary"].as<float>();


			auto entityCameraNode = data["EntityCamera"];
			DeserializeEntityCamera(entityCameraNode);
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[CameraComponent]: Could not deserialize component\n\t" + e.msg);
			return false;
		}
		return true;
	}

	void CameraComponent::DeserializeEntityCamera(YAML::Node& data)
	{
		camera.SetProjectionMode(StringToProjectionMode(data["ProjectionMode"].as<std::string>()));

		camera.SetPerspectiveFOV(data["PerspectiveFOV"].as<float>());
		camera.SetPerspectiveNearClip(data["PerspectiveNear"].as<float>());
		camera.SetPerspectiveFarClip(data["PerspectiveFar"].as<float>());
		camera.SetOrthographicSize(data["OrthographicSize"].as<float>());
		camera.SetOrthographicNearClip(data["OrthographicNear"].as<float>());
		camera.SetOrthographicFarClip(data["OrthographicFar"].as<float>());
	}
}
