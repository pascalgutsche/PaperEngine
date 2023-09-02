#pragma once
#include "utils/PaperID.h"

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
			node.SetStyle(EmitterStyle::Flow);
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
			node.SetStyle(EmitterStyle::Flow);
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
			node.SetStyle(EmitterStyle::Flow);
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

/*
	template <>
	struct convert<Vertex> {
		static Node encode(const Vertex& rhs)
		{
			Node node;
			node.push_back(rhs.position);
			node.push_back(rhs.normal);
			node.push_back(rhs.color);
			node.SetStyle(EmitterStyle::Flow);
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
*/

	template <>
	struct convert<Paper::PaperID> {
		static Node encode(const Paper::PaperID& rhs)
		{
			Node node;
			node.push_back(rhs.toString());
			return node;
		}

		static bool decode(const Node& node, Paper::PaperID& rhs)
		{
			rhs.Set(node[0].as<std::string>());
			return true;
		}
	};

	template <>
	struct convert<std::filesystem::path> {
		static Node encode(const std::filesystem::path& rhs)
		{
			Node node;
			node.push_back(rhs.string());
			return node;
		}

		static bool decode(const Node& node, std::filesystem::path& rhs)
		{
			rhs = node[0].as<std::string>();
			return true;
		}
	};

	template <>
	struct convert<std::array<glm::vec2, 4>> {
		static Node encode(const std::array<glm::vec2, 4>& rhs)
		{
			Node node;
			node.push_back(rhs[0].x);
			node.push_back(rhs[0].y);
			node.push_back(rhs[1].x);
			node.push_back(rhs[1].y);
			node.push_back(rhs[2].x);
			node.push_back(rhs[2].y);
			node.push_back(rhs[3].x);
			node.push_back(rhs[3].y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, std::array<glm::vec2, 4>& rhs)
		{
			if (!node.IsSequence() || node.size() != 8)
				return false;

			rhs[0].x = node[0].as<float>();
			rhs[0].y = node[1].as<float>();
			rhs[1].x = node[2].as<float>();
			rhs[1].y = node[3].as<float>();
			rhs[2].x = node[4].as<float>();
			rhs[2].y = node[5].as<float>();
			rhs[3].x = node[6].as<float>();
			rhs[3].y = node[7].as<float>();
			return true;
		}
	};
}

namespace Paper
{
	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const std::array<glm::vec2, 4>& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v[0].x << v[0].y << v[1].x << v[1].y << v[2].x << v[2].y << v[3].x << v[3].y << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const PaperID& uuid)
	{
		return out << YAML::BeginSeq << uuid.toString() << YAML::EndSeq;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const std::filesystem::path& path)
	{
		return out << path.string();
	}
}



