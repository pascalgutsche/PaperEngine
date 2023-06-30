#include "Engine.h"

#include "generic/Application.h"
#include "generic/Scene.h"

#include "renderer/Renderer2D.h"
#include "renderer/Renderer3D.h"

#include "renderer/RenderCommand.h"

#include "component/TransformComponent.h"
#include "component/SpriteComponent.h"
#include "component/CircleComponent.h"
#include "component/LineComponent.h"
#include "component/TextComponent.h"


namespace ppr {

	Scene::Scene()
		: uuid(UUID()), name("[Scene]"), is_dirty(true) { }

	Scene::Scene(const UUID& uuid)
		: uuid(uuid), name("[Scene]"), is_dirty(true) { }

	Scene::Scene(const std::string& name)
		: uuid(UUID()), name(name), is_dirty(true) { }

	Scene::Scene(const UUID& uuid, const std::string& name)
		: uuid(uuid), name(name), is_dirty(true) { }

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


	void Scene::Render(const Shr<EditorCamera>& camera)
	{
		RenderCommand::ClearStats();
		Renderer2D::BeginRender(camera);

		{
			auto view = registry.view<TransformComponent, SpriteComponent>();
			for (auto [entity, transform, sprite] : view.each()) {

				EdgeRenderData data;
				data.transform = transform.GetTransform();
				data.color = sprite.color;
				data.texture = sprite.texture;
				data.tilingFactor = sprite.tiling_factor;
				data.texCoords = sprite.tex_coords;
				data.coreIDToAlphaPixels = sprite.register_alpha_pixels_to_event;
				data.enity_id = (entity_id)entity;

				if (sprite.geometry == Geometry::RECTANGLE)
					Renderer2D::DrawRectangle(data);
				else if (sprite.geometry == Geometry::TRIANGLE)
					Renderer2D::DrawTriangle(data);
			}
		}

		{
			auto view = registry.view<TransformComponent, CircleComponent>();
			for (auto [entity, transform, circle] : view.each()) {
				
				CircleRenderData data;
				data.transform = transform.GetTransform();
				data.color = circle.color;
				data.texture = circle.texture;
				data.tilingFactor = circle.tiling_factor;
				data.coreIDToAlphaPixels = circle.register_alpha_pixels_to_event;
				data.enity_id = (entity_id)entity;

				Renderer2D::DrawCircle(data);
			}
		}

		{
			auto view = registry.view<TransformComponent, LineComponent>();
			for (auto [entity, transform, line] : view.each()) {

				LineRenderData data;
				data.point0 = line.positionA;
				data.point1 = line.positionB;
				data.color = line.color;
				data.thickness = line.thickness;
				data.enity_id = (entity_id)entity;
				
				Renderer2D::DrawLine(data);
			}
		}

		{
			auto view = registry.view<TransformComponent, TextComponent>();
			for (auto [entity, transform, text] : view.each()) {

				TextRenderData data;
				data.transform = transform.GetTransform();
				data.color = text.color;
				data.text = text.text;
				data.coreIDToAlphaPixels = text.register_alpha_pixels_to_event;
				data.enity_id = (entity_id)entity;
				
				Renderer2D::DrawString(data);
			}
		}
		Renderer2D::EndRender();
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		is_dirty = true;
		return CreateEntity(UUID(), name);
	}

	Entity Scene::CreateEntity(const UUID& id, const std::string& name)
	{
		is_dirty = true;
		Entity entity(registry.create(), id, name, this);
		entity_map[id] = entity;

		return entity;
	}

	bool Scene::DestroyEntity(Entity entity)
	{
		if (!entity_map.contains(entity.GetUUID())) return false;

		is_dirty = true;

		entity_map.erase(entity.GetUUID());
		registry.destroy(entity);
		return true;
	}

	Entity Scene::GetEntity(const UUID& id)
	{
		CORE_ASSERT(entity_map.contains(id), "Entity does not exists");
		
		return {entity_map.at(id), this};
	}
}
