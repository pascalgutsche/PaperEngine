#include "Engine.h"
#include "PhysicsComponent.h"

namespace Paper
{
	bool Rigidbody2DComponent::Serialize(YAML::Emitter& out)
	{
		try
		{
			out << YAML::Key << "Rigidbody2DComponent";
			out << YAML::BeginMap;
		
			out << YAML::Key << "Type" << BodyTypeToString(type);
			out << YAML::Key << "FixedRotation" << fixedRotation;

			out << YAML::EndMap;
			return true;
		}
		catch (YAML::Exception& e)
		{
			LOG_CORE_CRITICAL("[Rigidbody2DComponent]: Could not serialize component\n\t" + e.msg);
			return false;
		}
	}

	bool Rigidbody2DComponent::Deserialize(YAML::Node& data)
	{
		try
		{
			type = StringToBodyType(data["Type"].as<std::string>());
			fixedRotation = data["FixedRotation"].as<bool>();
		}
		catch (YAML::Exception& e)
		{
			LOG_CORE_CRITICAL("[Rigidbody2DComponent]: Could not deserialize component\n\t" + e.msg);
			return false;
		}
		return true;
	}

	bool Collider2DComponent::Serialize(YAML::Emitter& out)
	{
		try
		{
			out << YAML::Key << "Collider2DComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "Geometry" << YAML::Value << GeometryToString(geometry);
			out << YAML::Key << "Offset" << YAML::Value << offset;
			out << YAML::Key << "Size" << YAML::Value << size;
			out << YAML::Key << "Radius" << YAML::Value << radius;

			out << YAML::Key << "Density" << YAML::Value << density;
			out << YAML::Key << "Friction" << YAML::Value << friction;
			out << YAML::Key << "Bounciness" << YAML::Value << bounciness;
			out << YAML::Key << "BouncinessThreshold" << YAML::Value << bouncinessThreshold;

			out << YAML::EndMap;
			return true;
		}
		catch (YAML::Exception& e)
		{
			LOG_CORE_CRITICAL("[Collider2DComponent]: Could not serialize component\n\t" + e.msg);
			return false;
		}
	}

	bool Collider2DComponent::Deserialize(YAML::Node& data)
	{
		try
		{
			geometry = StringToGeometry(data["Geometry"].as<std::string>());
			offset = data["Offset"].as<glm::vec2>();
			size = data["Size"].as<glm::vec2>();
			radius = data["Radius"].as<float>();

			density = data["Density"].as<float>();
			friction = data["Friction"].as<float>();
			bounciness = data["Bounciness"].as<float>();
			bouncinessThreshold = data["BouncinessThreshold"].as<float>();

		}
		catch (YAML::Exception& e)
		{
			LOG_CORE_CRITICAL("[Collider2DComponent]: Could not deserialize component\n\t" + e.msg);
			return false;
		}
		return true;
	}
}
