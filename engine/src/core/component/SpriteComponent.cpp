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
			out << YAML::Key << "TextureHandle" << YAML::Value << textureHandle;
			out << YAML::Key << "TilingFactor" << YAML::Value << tilingFactor;
			out << YAML::Key << "TexCoords" << YAML::Value << texCoords;
			out << YAML::Key << "Geometry" << YAML::Value << GeometryToString(geometry);
			out << YAML::Key << "RegisterAlphaPixels" << YAML::Value << registerAlphaPixelsToEvent;

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
			textureHandle = data["TextureHandle"].as<AssetHandle>();
			tilingFactor = data["TilingFactor"].as<float>();
			texCoords = data["TexCoords"].as<std::array<glm::vec2, 4>>();
			geometry = StringToGeometry(data["Geometry"].as<std::string>());
			registerAlphaPixelsToEvent = data["RegisterAlphaPixels"].as<bool>();

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
