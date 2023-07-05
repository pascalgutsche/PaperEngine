#include "Engine.h"
#include "YAMLSerializer.h"

#include "generic/Entity.h"

#include "Components.h"

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
		//YAML::Emitter out;
		//out << YAML::BeginMap;
		//out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		//out << YAML::Key << "TextComponent";
		//out << YAML::BeginMap << "TextComponent"; // TextComponent
		//
		//out << YAML::Key << "TextString" << YAML::Value << "textComponent.TextString";
		//out << YAML::Key << "Color" << YAML::Value << "textComponent.Color";
		//out << YAML::Key << "Kerning" << YAML::Value << 795348;
		//out << YAML::Key << "LineSpacing" << YAML::Value << 59435.957;
		//
		//out << YAML::EndMap; // TextComponent
		//out << YAML::EndSeq;
		//out << YAML::EndMap;
		//
		//std::ofstream fout(filePath);
		//fout << out.c_str();
		//fout.close();
		//
		return true;
	}

	bool YAMLSerializer::SceneSerialize(const std::filesystem::path& filePath, const Shr<Scene>& scene)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << scene->GetUUID();
		out << YAML::Key << "Name" << YAML::Value << scene->GetName();

		std::string scene_path = scene->GetPath().string();
		const std::string abs_path = std::filesystem::current_path().string() + "\\";
		const size_t pos = scene_path.find(abs_path);
		ASSERT(pos != std::string::npos, "")
		const std::filesystem::path path(scene_path.erase(pos, abs_path.length()));

		
		out << YAML::Key << "Path" << YAML::Value << path.string();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		scene->registry.each([&](auto entityID)
			{
				Entity entity = { entityID, scene.get() };
				if (!entity)
					return;

				EntitySerialize(entity, out);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;


		std::ofstream fout(filePath);
		fout << out.c_str();
		fout.close();

		return true;
	}

	bool YAMLSerializer::EntitySerialize(Entity& entity, YAML::Emitter& out)
	{
		CORE_ASSERT(entity.HasComponent<DataComponent>(), "")

		out << YAML::BeginMap; // Entity
		//out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();
		//out << YAML::Key << "Name" << YAML::Value << entity.GetName();

		out << YAML::Key << "Components";
		out << YAML::BeginMap;

		if (entity.HasComponent<DataComponent>())
			entity.GetComponent<DataComponent>().Serialize(out);

		if (entity.HasComponent<CircleComponent>())
			entity.GetComponent<CircleComponent>().Serialize(out);

		if (entity.HasComponent<LineComponent>())
			entity.GetComponent<LineComponent>().Serialize(out);

		if (entity.HasComponent<SpriteComponent>())
			entity.GetComponent<SpriteComponent>().Serialize(out);

		if (entity.HasComponent<TextComponent>())
			entity.GetComponent<TextComponent>().Serialize(out);

		if (entity.HasComponent<TransformComponent>())
			entity.GetComponent<TransformComponent>().Serialize(out);

		out << YAML::EndMap;
		out << YAML::EndMap; // Entity

		std::ofstream fout("lol.yaml");
		fout << out.c_str();
		fout.close();

		return true;
	}

	Shr<Scene> YAMLSerializer::SceneDeserialize(const std::filesystem::path& filePath)
	{
		Shr<Scene> scene = MakeShr<Scene>();

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filePath.string());
		}
		catch (YAML::ParserException e)
		{
			LOG_CORE_ERROR("Failed to load file '{0}'\n\t{1}", filePath, e.what());
			return nullptr;
		}

		if (!data["Scene"])
			return nullptr;

		std::string scene_name = data["Name"].as<std::string>();
		LOG_CORE_TRACE("Deserializing scene '{0}' from '{1}'", scene_name, filePath.string());

		scene->name = scene_name;
		if (data["Path"])
			scene->path = data["Path"].as<std::string>();

		if (auto entities = data["Entities"])
		{
			for (auto entity : entities)
			{
				CORE_ASSERT(entity["Components"], "")
				auto components = entity["Components"];

				UUID uuid;
				std::string entity_name;
				if (auto data_component = components["DataComponent"])
				{
					uuid = data_component["UUID"].as<UUID>();
					entity_name = data_component["Name"].as<std::string>();
				}
				
				LOG_CORE_TRACE("Deserializing entity name '{0}' and uuid '{1}' from '{2}'", entity_name, uuid.toString(), filePath.string());
				
				Entity deserialized_entity = scene->CreateEntity(uuid, entity_name);
				
				if (auto transformComponent = components["TransformComponent"])
					deserialized_entity.GetComponent<TransformComponent>().Deserialize(transformComponent);
				
				if (auto sprite_component = components["SpriteComponent"])
					deserialized_entity.AddComponent<SpriteComponent>().Deserialize(sprite_component);
				
				if (auto circle_component = components["CircleComponent"])
					deserialized_entity.AddComponent<CircleComponent>().Deserialize(circle_component);
				
				if (auto line_component = components["LineComponent"])
					deserialized_entity.AddComponent<LineComponent>().Deserialize(line_component);
				
				if (auto text_component = entity["TextComponent"])
					deserialized_entity.AddComponent<TextComponent>().Deserialize(text_component);
			}
		}
		//scene->SetClean();
		return scene;
	}
}


