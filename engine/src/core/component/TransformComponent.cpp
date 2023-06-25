#include "Engine.h"
#include "TransformComponent.h"

namespace ppr
{
	bool TransformComponent::Serialize(YAML::Emitter& out)
	{
		try
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // SpriteComponent

			out << YAML::Key << "Position" << YAML::Value << position;
			out << YAML::Key << "Scale" << YAML::Value << scale;
			out << YAML::Key << "Rotation" << YAML::Value << rotation;

			out << YAML::EndMap; // SpriteComponent
			return true;
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[TransformComponent]: Could not serialize component\n\t" + e.msg);
			return false;
		}
	}

	bool TransformComponent::Deserialize(YAML::Node& data)
	{
		try
		{
			position = data["Position"].as<glm::vec3>();
			scale = data["Scale"].as<glm::vec3>();
			rotation = data["Rotation"].as<glm::vec3>();
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[TransformComponent]: Could not deserialize component\n\t" + e.msg);
			return false;
		}
		return true;
	}
}