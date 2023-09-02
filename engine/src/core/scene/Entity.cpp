#include "Engine.h"

#include "Entity.h"

#include "component/DataComponent.h"
#include "component/TransformComponent.h"


namespace Paper {
	Entity::Entity(entt::entity entity, Scene* scene)
		: scene(scene), entity(entity) { }

	Entity::Entity(entt::entity entity, std::string name, Scene* scene)
		: scene(scene), entity(entity)
	{
		AddComponent<DataComponent>(name);
		AddComponent<TransformComponent>();
	}

	Entity::Entity(entt::entity entity, const PaperID& id, std::string name, Scene* scene)
		: scene(scene), entity(entity)
	{
		AddComponent<DataComponent>(id, name);
		AddComponent<TransformComponent>();

	}

	Entity* Entity::AddTag(std::string tag)
	{
		auto& tagList = GetComponent<DataComponent>().tags;
		std::ranges::transform(tag.begin(), tag.end(), tag.begin(), ::toupper);
		if (std::ranges::find(tagList.begin(), tagList.end(), tag) != tagList.end())
		{
			LOG_CORE_WARN("Adding a tag to a Entity which it already has: '" + tag + "'");
			return this;
		}
		tagList.emplace_back(tag);
		return this;
	}

	Entity* Entity::AddTag(std::initializer_list<std::string> tags)
	{
		for (std::string tag : tags) {
			std::ranges::transform(tag.begin(), tag.end(), tag.begin(), ::toupper);
			AddTag(tag);
		}
		return this;
	}

	bool Entity::RemoveTag(std::string tag)
	{
		auto& tagList = GetComponent<DataComponent>().tags;
		std::ranges::transform(tag.begin(), tag.end(), tag.begin(), ::toupper);
		const auto it = std::ranges::find(tagList.begin(), tagList.end(), tag);
		if (it == tagList.end())
		{
			LOG_CORE_WARN("Removing a tag from a Entity which it doesn't have: '" + tag + "'");
			return false;
		}
		tagList.erase(it);
		return true;
	}

	bool Entity::HasTag(std::string tag)
	{
		auto& tagList = GetComponent<DataComponent>().tags;
		std::ranges::transform(tag.begin(), tag.end(), tag.begin(), ::toupper);
		const auto it = std::ranges::find(tagList.begin(), tagList.end(), tag);
		return it != tagList.end();
	}

	PaperID Entity::GetPaperID()
	{
		if (!*this)
			return PaperID(0);
		return GetComponent<DataComponent>().uuid;
	}

	std::string Entity::GetName()
	{
		return GetComponent<DataComponent>().name;
	}

	void Entity::SetName(const std::string& name)
	{
		GetComponent<DataComponent>().name = name;
	}
}