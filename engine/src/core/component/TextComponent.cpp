#include "Engine.h"
#include "TextComponent.h"

#include "renderer/Font.h"

namespace ppr
{
	bool TextComponent::Serialize(YAML::Emitter& out)
	{
		try
		{
			out << YAML::Key << "TextComponent";
			out << YAML::BeginMap; // SpriteComponent

			out << YAML::Key << "Color" << YAML::Value << color;
			out << YAML::Key << "Text" << YAML::Value << text;
			out << YAML::Key << "FontPath" << YAML::Value << font->GetFilePath();
			out << YAML::Key << "RegisterAlphaPixels" << YAML::Value << register_alpha_pixels_to_event;

			out << YAML::EndMap; // SpriteComponent
			return true;
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[TextComponent]: Could not serialize component\n\t" + e.msg);
			return false;
		}
	}

	bool TextComponent::Deserialize(YAML::Node& data)
	{
		try
		{
			color = data["Color"].as<glm::vec4>();
			text = data["Text"].as<std::string>();
			font = DataPool::GetFont(data["FontPath"].as<std::string>());
			register_alpha_pixels_to_event = data["RegisterAlphaPixels"].as<bool>();
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[TextComponent]: Could not deserialize component\n\t" + e.msg);
			return false;
		}
		return true;
	}
}
