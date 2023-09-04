#include "Engine.h"
#include "ProjectSerializer.h"

#include "yaml-cpp/yaml.h"

namespace Paper
{
	void ProjectSerializer::Serialize(const Shr<Project>& project, const std::filesystem::path& filePath)
	{
		const ProjectConfig& config = project->config;

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "ProjectConfig";
		out << YAML::BeginMap;

		out << YAML::Key << "Name" << YAML::Value << config.projectName;
		out << YAML::Key << "Path" << YAML::Value << config.projectPath.string();
		out << YAML::Key << "AssetPath" << YAML::Value << config.assetPath.string();
		out << YAML::Key << "ScriptBinaryPath" << YAML::Value << config.scriptBinaryPath.string();
		out << YAML::Key << "StartScene" << YAML::Value << config.startScene.string();

		out << YAML::EndMap;
		out << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << out.c_str();
		fout.close();
	}

	Shr<Project> ProjectSerializer::Deserialize(const std::filesystem::path& filePath)
	{
		Shr<Project> project = MakeShr<Project>();
		ProjectConfig& config = project->config;
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filePath.string());
		}
		catch (YAML::Exception e)
		{
			LOG_CORE_ERROR("Failed to load project file '{0}'\n\t{1}", filePath, e.what());
			return nullptr;
		}

		try
		{
			if (!data["ProjectConfig"])
				return nullptr;

			auto configNode = data["ProjectConfig"];

			config.projectName = configNode["Name"].as<std::string>();
			config.projectPath = configNode["Path"].as<std::string>();
			config.assetPath = configNode["AssetPath"].as<std::string>();
			config.scriptBinaryPath = configNode["ScriptBinaryPath"].as<std::string>();

			if (configNode["StartScene"])
				config.startScene = configNode["StartScene"].as<std::string>();

		}
		catch (YAML::Exception& ex)
		{
			LOG_CORE_CRITICAL(ex.msg);
			return nullptr;
		}

		return project;
	}
}