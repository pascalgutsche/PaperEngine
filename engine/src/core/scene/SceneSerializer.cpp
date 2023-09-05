#include "Engine.h"
#include "SceneSerializer.h"

#include "serializer/YAMLSerializer.h"

#include "Components.h"
#include "Entity.h"

#include "scripting/ScriptEngine.h"
#include "utils/Utils.h"

namespace Paper
{
	template <typename... Component>
	static void SerializeEntity(Entity entity, YAML::Emitter& out)
	{
		([&]()
		{
			static_assert(std::is_base_of<Serializable, Component>(), "Component has to derive from Serializable");
			if (entity.HasComponent<Component>())
			{
				Serializable& serializable = (Serializable&)entity.GetComponent<Component>();
				serializable.serializedEntity = &entity;
				serializable.Serialize(out);
				serializable.serializedEntity = nullptr;
			}
		}(), ...);
	}

	template<typename... Component>
	static void SerializeEntity(ComponentGroup<Component...>, Entity entity, YAML::Emitter& out)
	{
		SerializeEntity<Component...>(entity, out);
	}

	bool EntitySerialize(Entity& entity, YAML::Emitter& out)
	{
		CORE_ASSERT(entity.HasComponent<DataComponent>(), "")

		out << YAML::BeginMap; // Entity
		//out << YAML::Key << "Entity" << YAML::Value << entity.GetPaperID();
		//out << YAML::Key << "Name" << YAML::Value << entity.GetName();

		out << YAML::Key << "Components";
		out << YAML::BeginMap;

		if (entity.HasComponent<DataComponent>())
			entity.GetComponent<DataComponent>().Serialize(out);

		SerializeEntity(AllComponents{}, entity, out);

		//if (entity.HasComponent<LineComponent>())
		//	entity.GetComponent<LineComponent>().Serialize(out);
		//
		//if (entity.HasComponent<SpriteComponent>())
		//	entity.GetComponent<SpriteComponent>().Serialize(out);
		//
		//if (entity.HasComponent<TextComponent>())
		//	entity.GetComponent<TextComponent>().Serialize(out);
		//
		//if (entity.HasComponent<ScriptComponent>())
		//{
		//	entity.GetComponent<ScriptComponent>().Serialize(out);
		//
		//	
		//}
		//
		//if (entity.HasComponent<TransformComponent>())
		//	entity.GetComponent<TransformComponent>().Serialize(out);
		//
		//if (entity.HasComponent<CameraComponent>())
		//	entity.GetComponent<CameraComponent>().Serialize(out);

		out << YAML::EndMap;
		out << YAML::EndMap; // Entity

		return true;
	}

	void SceneSerializer::Serialize(const Shr<Scene>& scene, const std::filesystem::path& filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << scene->GetPaperID();
		out << YAML::Key << "Name" << YAML::Value << scene->GetName();

		std::string scene_path = scene->GetPath().string();
		const std::string abs_path = std::filesystem::current_path().string() + "\\";
		const size_t pos = scene_path.find(abs_path);
		std::filesystem::path path(scene_path);
		if (pos != std::string::npos)
			path = std::filesystem::path(scene_path.erase(pos, abs_path.length()));


		out << YAML::Key << "Path" << YAML::Value << path.string();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		scene->registry.each([&](auto PaperID)
			{
				Entity entity = { PaperID, scene.get() };
				if (!entity)
					return;

				EntitySerialize(entity, out);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;


		std::ofstream fout(filePath);
		fout << out.c_str();
		fout.close();
	}

	template <typename... Component>
	static void DeserializeEntity(Entity entity, YAML::Node& data)
	{
		([&]()
			{
				static_assert(std::is_base_of<Serializable, Component>(), "Component has to derive from Serializable");

				if (auto componentNode = data[Utils::TypeToStdString<Component>()])
				{
					Serializable& serializable = (Serializable&)entity.AddOrReplaceComponent<Component>();
					serializable.serializedEntity = &entity;
					serializable.Deserialize(componentNode);
					serializable.serializedEntity = nullptr;
				}
			}(), ...);
	}

	template<typename... Component>
	static void DeserializeEntity(ComponentGroup<Component...>, Entity entity, YAML::Node& data)
	{
		DeserializeEntity<Component...>(entity, data);
	}

	

	Shr<Scene> SceneSerializer::Deserialize(const std::filesystem::path& filePath)
	{
		Shr<Scene> scene = MakeShr<Scene>();

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filePath.string());
		}
		catch (YAML::Exception e)
		{
			LOG_CORE_ERROR("Failed to load file '{0}'\n\t{1}", filePath, e.what());
			return nullptr;
		}

		try
		{
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

					PaperID uuid;
					std::string entity_name;
					if (auto data_component = components["DataComponent"])
					{
						uuid = data_component["PaperID"].as<PaperID>();
						entity_name = data_component["Name"].as<std::string>();
					}

					LOG_CORE_TRACE("Deserializing entity name '{0}' and uuid '{1}' from '{2}'", entity_name, uuid.toString(), filePath.string());

					Entity deserialized_entity = scene->CreateEntity(uuid, entity_name);

					DeserializeEntity(AllComponents{}, deserialized_entity, components);

					//if (auto transformComponent = components["TransformComponent"])
					//	deserialized_entity.GetComponent<TransformComponent>().Deserialize(transformComponent);
					//
					//if (auto sprite_component = components["SpriteComponent"])
					//	deserialized_entity.AddComponent<SpriteComponent>().Deserialize(sprite_component);
					//
					//if (auto line_component = components["LineComponent"])
					//	deserialized_entity.AddComponent<LineComponent>().Deserialize(line_component);
					//
					//if (auto text_component = components["TextComponent"])
					//	deserialized_entity.AddComponent<TextComponent>().Deserialize(text_component);
					//
					//if (auto script_component = components["ScriptComponent"])
					//{
					//	deserialized_entity.AddComponent<ScriptComponent>().Deserialize(script_component);
					//}
					//
					//if (auto camera_component = components["CameraComponent"])
					//	deserialized_entity.AddComponent<CameraComponent>().Deserialize(camera_component);
				}
			}
		}
		catch (YAML::Exception& ex)
		{
			LOG_CORE_CRITICAL(ex.msg);
			return nullptr;
		}
		scene->SetClean();
		return scene;
	}
}
