#include "Engine.h"
#include "YAMLSerializer.h"

#include "yaml-cpp/yaml.h"

#include <fstream>

namespace YAML
{
	template <>
	Node convert<>::encode(const glm::vec2& rhs)
	{
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		return node;
	}

	template <>
	bool convert<>::decode(const Node& node, glm::vec2& rhs)
	{
		if (!node.IsSequence() || node.size() != 2)
			return false;

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		return true;
	}

	Node encode(const glm::vec3& rhs)
	{
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		return node;
	}

	bool decode(const Node& node, glm::vec3& rhs)
	{
		if (!node.IsSequence() || node.size() != 3)
			return false;

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();
		return true;
	}

	template <>
	struct convert<glm::vec4> {
		Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template <>
	struct convert<Vertex> {
		Node encode(const Vertex& rhs)
		{
			Node node;
			node.push_back(rhs.position);
			node.push_back(rhs.normal);
			node.push_back(rhs.color);
			return node;
		}

		bool decode(const Node& node, Vertex& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.position = node[0].as<glm::vec3>();
			rhs.normal = node[1].as<glm::vec3>();
			rhs.color = node[2].as<glm::vec4>();
			return true;
		}
	};
}

namespace engine
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
		out << YAML::BeginMap; // TextComponent

		out << YAML::Key << "TextString" << YAML::Value << "textComponent.TextString";
		// TODO: textComponent.FontAsset
		out << YAML::Key << "Color" << YAML::Value << "textComponent.Color" << 3495;
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
	
}


