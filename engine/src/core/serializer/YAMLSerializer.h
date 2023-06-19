#pragma once
#include "Engine.h"

#include "assets/Asset3D.h"
#include "generic/Scene.h"

#include "yaml-cpp/yaml.h"

namespace YAML
{
	template <>
	struct convert<glm::vec2> {
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template <>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template <>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
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
		static Node encode(const Vertex& rhs)
		{
			Node node;
			node.push_back(rhs.position);
			node.push_back(rhs.normal);
			node.push_back(rhs.color);
			return node;
		}

		static bool decode(const Node& node, Vertex& rhs)
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
	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	class YAMLSerializer
	{
	public:
		YAMLSerializer();
		~YAMLSerializer();

		bool AssetSerialize(const std::filesystem::path& filePath, const Asset3D& asset);

		bool SceneSerializer(const Scene& scene);
	};
}



