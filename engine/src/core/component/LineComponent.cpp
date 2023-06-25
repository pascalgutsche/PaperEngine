#include "Engine.h"
#include "LineComponent.h"

namespace ppr
{
	bool LineComponent::Serialize(YAML::Emitter& out)
	{
		try
		{
			out << YAML::Key << "LineComponent";
			out << YAML::BeginMap; // SpriteComponent

			out << YAML::Key << "Color" << YAML::Value << color;
			out << YAML::Key << "Thickness" << YAML::Value << thickness;
			out << YAML::Key << "PositionA" << YAML::Value << positionA;
			out << YAML::Key << "PositionB" << YAML::Value << positionB;

			out << YAML::EndMap; // SpriteComponent
			return true;
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[LineComponent]: Could not serialize component\n\t" + e.msg);
			return false;
		}
	}

	bool LineComponent::Deserialize(YAML::Node& data)
	{
		try
		{
			color = data["Color"].as<glm::vec4>();
			thickness = data["Thickness"].as<float>();
			positionA = data["PositionA"].as<glm::vec3>();
			positionB = data["PositionB"].as<glm::vec3>();
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[LineComponent]: Could not deserialize component\n\t" + e.msg);
			return false;
		}
		return true;
	}
}
