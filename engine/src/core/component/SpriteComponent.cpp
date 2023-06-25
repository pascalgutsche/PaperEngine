#include "Engine.h"
#include "SpriteComponent.h"

namespace ppr
{

	bool SpriteComponent::Serialize(YAML::Emitter& out)
	{
		try 
		{
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap; // SpriteComponent

			out << YAML::Key << "Color" << YAML::Value << color;
			if (texture)
				out << YAML::Key << "TexturePath" << YAML::Value << texture->GetFilePath();
			out << YAML::Key << "TilingFactor" << YAML::Value << tiling_factor;
			out << YAML::Key << "TexCoords" << YAML::Value << tex_coords;
			out << YAML::Key << "RegisterAlphaPixels" << YAML::Value << register_alpha_pixels_to_event;

			out << YAML::EndMap; // SpriteComponent
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[SpriteComponent]: Could not serialize component\n\t" + e.msg);
			return false;
		}
		return true;
	}

	bool SpriteComponent::Deserialize(YAML::Node& data)
	{
		try 
		{
			color = data["Color"].as<glm::vec4>();
			if (data["TexturePath"])
				texture = DataPool::GetTexture(data["TexturePath"].as<std::string>());
			tiling_factor = data["TilingFactor"].as<float>();
			tex_coords = data["TexCoords"].as<std::array<glm::vec2, 4>>();
			register_alpha_pixels_to_event = data["RegisterAlphaPixels"].as<bool>();
		}
		catch (YAML::EmitterException& e)
		{
			LOG_CORE_CRITICAL("[SpriteComponent]: Could not deserialize component\n\t" + e.msg);
			return false;
		}
		return true;
	}
}
