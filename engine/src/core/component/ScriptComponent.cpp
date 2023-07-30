#include "Engine.h"
#include "ScriptComponent.h"


namespace Paper
{
	bool ScriptComponent::Serialize(YAML::Emitter& out)
	{
		try
		{
			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap; // SpriteComponent

			out << YAML::Key << "Name" << YAML::Value << name;

			out << YAML::EndMap; // SpriteComponent
			return true;
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[ScriptComponent]: Could not serialize component\n\t" + e.msg);
			return false;
		}
	}

	bool ScriptComponent::Deserialize(YAML::Node& data)
	{
		try
		{
			name = data["Name"].as<std::string>();
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[ScriptComponent]: Could not deserialize component\n\t" + e.msg);
			return false;
		}
		return true;
	}
}
