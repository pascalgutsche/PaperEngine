#include "Engine.h"
#include "DataComponent.h"

namespace Paper
{
	bool DataComponent::Serialize(YAML::Emitter& out)
	{
		try
		{
			out << YAML::Key << "DataComponent";
			out << YAML::BeginMap; // SpriteComponent

			out << YAML::Key << "EntityID" << YAML::Value << uuid;
			out << YAML::Key << "Name" << YAML::Value << name;
			out << YAML::Key << "Tags" << YAML::Value << tags;

			out << YAML::EndMap; // SpriteComponent
			return true;
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[DataComponent]: Could not serialize component\n\t" + e.msg);
			return false;
		}
	}

	bool DataComponent::Deserialize(YAML::Node& data)
	{
		try
		{
			uuid = data["EntityID"].as<EntityID>();
			name = data["Name"].as<std::string>();
			tags = data["Tags"].as<std::vector<std::string>>();
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[DataComponent]: Could not deserialize component\n\t" + e.msg);
			return false;
		}
		return true;
	}
}
