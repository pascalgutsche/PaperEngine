#include "Engine.h"
#include "SpriteComponent.h"

#include "project/Project.h"

namespace Paper
{

	bool SpriteComponent::Serialize(YAML::Emitter& out)
	{
		try 
		{
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap; // SpriteComponent

			out << YAML::Key << "Color" << YAML::Value << color;
			if (texture)
				out << YAML::Key << "TexturePath" << YAML::Value << Project::GetRelativePathFromProject(texture->GetFilePath());
			out << YAML::Key << "TilingFactor" << YAML::Value << tiling_factor;
			out << YAML::Key << "TexCoords" << YAML::Value << tex_coords;
			out << YAML::Key << "Geometry" << YAML::Value << GeometryToString(geometry);
			out << YAML::Key << "RegisterAlphaPixels" << YAML::Value << register_alpha_pixels_to_event;

			out << YAML::Key << "Thickness" << YAML::Value << thickness;
			out << YAML::Key << "Fade" << YAML::Value << fade;

			out << YAML::EndMap; // SpriteComponent
		}
		catch (YAML::Exception& e)
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
				texture = DataPool::GetAssetTexture(data["TexturePath"].as<std::string>(), true);
			tiling_factor = data["TilingFactor"].as<float>();
			tex_coords = data["TexCoords"].as<std::array<glm::vec2, 4>>();
			geometry = StringToGeometry(data["Geometry"].as<std::string>());
			register_alpha_pixels_to_event = data["RegisterAlphaPixels"].as<bool>();

			thickness = data["Thickness"].as<float>();
			fade = data["Fade"].as<float>();
		}
		catch (YAML::Exception& e)
		{
			LOG_CORE_CRITICAL("[SpriteComponent]: Could not deserialize component\n\t" + e.msg);
			return false;
		}
		return true;
	}
}
