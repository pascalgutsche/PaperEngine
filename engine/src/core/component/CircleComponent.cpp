#include "Engine.h"
#include "CircleComponent.h"

#include "utils/DataPool.h"

namespace Paper
{
	bool CircleComponent::Serialize(YAML::Emitter& out)
	{
		try
		{
			out << YAML::Key << "CircleComponent";
			out << YAML::BeginMap; // SpriteComponent

			out << YAML::Key << "Color" << YAML::Value << color;
			if (texture)
				out << YAML::Key << "TexturePath" << YAML::Value << texture->GetFilePath();
			out << YAML::Key << "TilingFactor" << YAML::Value << tiling_factor;
			out << YAML::Key << "Thickness" << YAML::Value << thickness;
			out << YAML::Key << "Fade" << YAML::Value << fade;
			out << YAML::Key << "RegisterAlphaPixels" << YAML::Value << register_alpha_pixels_to_event;

			out << YAML::EndMap; // SpriteComponent
			return true;
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[CircleComponent]: Could not serialize component\n\t" + e.msg);
			return false;
		}
	}

	bool CircleComponent::Deserialize(YAML::Node& data)
	{
		try
		{
			color = data["Color"].as<glm::vec4>();
			if (data["TexturePath"])
				texture = DataPool::GetAssetTexture(data["TexturePath"].as<std::string>(), true);
			tiling_factor = data["TilingFactor"].as<float>();
			thickness = data["Thickness"].as<float>();
			fade = data["Fade"].as<float>();
			register_alpha_pixels_to_event = data["RegisterAlphaPixels"].as<bool>();
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[CircleComponent]: Could not deserialize component\n\t" + e.msg);
			return false;
		}
		return true;
	}
}
