#include "Engine.h"

#include "generic/Application.h"
#include "generic/Scene.h"

#include "generic/Camera.h"
#include "renderer/Renderer2D.h"
#include "renderer/Renderer3D.h"

#include "component/TransformComponent.h"
#include "component/SpriteComponent.h"
#include "component/CircleComponent.h"
#include "component/LineComponent.h"
#include "component/TextComponent.h"


namespace ppr {

	Scene::Scene()
		: uuid(UUID()), name("[Scene]") { }

	Scene::Scene(const UUID& uuid)
		: uuid(uuid), name("[Scene]") { }

	Scene::Scene(const std::string& name)
		: uuid(UUID()), name(name) { }

	Scene::Scene(const UUID& uuid, const std::string& name)
		: uuid(uuid), name(name) { }

	Scene::~Scene()
	{
		registry.clear();
	}

	void Scene::Start()
	{
	}

	void Scene::Stop()
	{
	}


	void Scene::Render()
	{
		for (const auto sprite_entities = registry.group<TransformComponent>(entt::get<SpriteComponent>); auto & entity : sprite_entities)
		{
			auto [transform, sprite] = sprite_entities.get<TransformComponent, SpriteComponent>(entity);


			EdgeRenderData data;
			data.transform = transform.GetTransform();
			data.color = sprite.color;
			data.texture = sprite.texture;
			data.tilingFactor = sprite.tiling_factor;
			data.texCoords = sprite.tex_coords;
			data.coreIDToAlphaPixels = sprite.register_alpha_pixels_to_event;

			if (sprite.geometry == Geometry::RECTANGLE)
				Renderer2D::DrawRectangle(data);
			else if (sprite.geometry == Geometry::TRIANGLE)
				Renderer2D::DrawTriangle(data);
		}

		for (const auto circle_entities = registry.group<TransformComponent>(entt::get<CircleComponent>); auto & entity : circle_entities)
		{
			auto [transform, circle] = circle_entities.get<TransformComponent, CircleComponent>(entity);


			CircleRenderData data;
			data.transform = transform.GetTransform();
			data.color = circle.color;
			data.texture = circle.texture;
			data.tilingFactor = circle.tiling_factor;
			data.coreIDToAlphaPixels = circle.register_alpha_pixels_to_event;

			Renderer2D::DrawCircle(data);
		}

		for (const auto line_entities = registry.group<TransformComponent>(entt::get<LineComponent>); auto & entity : line_entities)
		{
			auto [transform, line] = line_entities.get<TransformComponent, LineComponent>(entity);


			LineRenderData data;
			data.point0 = line.positionA;
			data.point1 = line.positionB;
			data.color = line.color;
			data.thickness = line.thickness;

			Renderer2D::DrawLine(data);
		}

		for (const auto text_entities = registry.group<TransformComponent>(entt::get<TextComponent>); auto & entity : text_entities)
		{
			auto [transform, text] = text_entities.get<TransformComponent, TextComponent>(entity);


			TextRenderData data;
			data.transform = transform.GetTransform();
			data.color = text.color;
			data.text = text.text;
			data.coreIDToAlphaPixels = text.register_alpha_pixels_to_event;

			Renderer2D::DrawString(data);
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntity(UUID(), name);
	}

	Entity Scene::CreateEntity(const UUID& id, const std::string& name)
	{
		Entity entity(registry.create(), id, name, this);
		entityMap[id] = entity;

		return entity;
	}

	bool Scene::DestroyEntity(Entity entity)
	{
		if (!entityMap.contains(entity.GetUUID())) return false;

		entityMap.erase(entity.GetUUID());
		registry.destroy(entity);
		return true;
	}

	Entity Scene::GetEntity(const UUID& id)
	{
		CORE_ASSERT(entityMap.contains(id), "Entity does not exists");
		
		return {entityMap.at(id), this};
	}
}
