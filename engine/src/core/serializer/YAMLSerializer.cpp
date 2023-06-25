#include "Engine.h"
#include "YAMLSerializer.h"

#include "generic/Entity.h"

#include "component/DataComponent.h"


namespace ppr
{
	YAMLSerializer::YAMLSerializer()
	{
	}

	YAMLSerializer::~YAMLSerializer()
	{
	}

	bool YAMLSerializer::AssetSerialize(const std::filesystem::path& filePath, const Asset3D& asset)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		out << YAML::Key << "TextComponent";
		out << YAML::BeginMap << "TextComponent"; // TextComponent

		out << YAML::Key << "TextString" << YAML::Value << "textComponent.TextString";
		out << YAML::Key << "Color" << YAML::Value << "textComponent.Color";
		out << YAML::Key << "Kerning" << YAML::Value << 795348;
		out << YAML::Key << "LineSpacing" << YAML::Value << 59435.957;

		out << YAML::EndMap; // TextComponent
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << out.c_str();
		fout.close();

		return true;
	}

	bool YAMLSerializer::SceneSerializer(const std::filesystem::path& filePath, const Scene& scene) const
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << YAML::BeginSeq;
		out << YAML::Key << "Name" << YAML::Value << scene.name;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		out << YAML::Key << "TextComponent";
		out << YAML::BeginMap << "TextComponent"; // TextComponent

		out << YAML::Key << "TextString" << YAML::Value << "textComponent.TextString";
		out << YAML::Key << "Color" << YAML::Value << "textComponent.Color";
		out << YAML::Key << "Kerning" << YAML::Value << 795348;
		out << YAML::Key << "LineSpacing" << YAML::Value << 59435.957;

		out << YAML::EndMap; // TextComponent
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << out.c_str();
		fout.close();

		return true;
	}

	bool YAMLSerializer::EntitySerialize(Entity& entity, YAML::Emitter& out) const
	{
		DataComponent& data = entity.GetComponent<DataComponent>();
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << YAML::BeginSeq;
		out << YAML::Key << "Name" << YAML::Value << data.name;
		out << YAML::Key << "Components";
		out << YAML::BeginMap << "TextComponent"; // TextComponent

		out << YAML::Key << "TextString" << YAML::Value << "textComponent.TextString";
		out << YAML::Key << "Color" << YAML::Value << "textComponent.Color";
		out << YAML::Key << "Kerning" << YAML::Value << 795348;
		out << YAML::Key << "LineSpacing" << YAML::Value << 59435.957;

		out << YAML::EndMap; // TextComponent
		out << YAML::EndSeq;
		out << YAML::EndMap;
		return true;
	}
}


