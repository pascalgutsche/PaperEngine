#include "Engine.h"

#include "Scene.h"
#include "Entity.h"

#include "camera/EntityCamera.h"
#include "generic/Application.h"
#include "renderer/Renderer2D.h"
#include "renderer/Renderer3D.h"
#include "scripting/ScriptEngine.h"

#include "Components.h"
#include "box2d/b2_body.h"
#include "box2d/b2_circle_shape.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_world.h"

namespace Paper {

	Scene::Scene(const PaperID& uuid)
		: config{ .uuid = uuid } { }

	Scene::Scene(const std::string& name)
		: config{ .name = name } { }

	Scene::Scene(const PaperID& uuid, const std::string& name)
		: config{ .uuid = uuid, .name = name } { }

	Scene::~Scene()
	{
		registry.clear();
	}

	template <typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<PaperID, entt::entity>& dstEntityMap)
	{
		([&]()
		{
			auto view = src.view<Component>();
			for (auto [e, component] : view.each())
			{
				PaperID srcUUID = src.get<DataComponent>(e).uuid;
				entt::entity dstEntity = dstEntityMap.at(srcUUID);

				dst.emplace_or_replace<Component>(dstEntity, component);
			}
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<PaperID, entt::entity>& dstEntityMap)
	{
		CopyComponent<Component...>(dst, src, dstEntityMap);
	}

	template <typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
		{
			if (src.HasComponent<Component>())
				dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}

	Shr<Scene> Scene::Copy()
	{
		Shr<Scene> newScene = MakeShr<Scene>();

		auto& dstSceneRegistry = newScene->registry;

		auto dataView = registry.view<DataComponent>();
		for (auto e : dataView)
		{
			PaperID uuid = registry.get<DataComponent>(e).uuid;
			std::string name = registry.get<DataComponent>(e).name;
			auto tags = registry.get<DataComponent>(e).tags;

			Entity entity = newScene->CreateEntity(uuid, name);
			entity.GetComponent<DataComponent>().tags = tags;
		}

		CopyComponent(AllComponents{},dstSceneRegistry, registry, newScene->entity_map);

		newScene->SetConfig(config);

		return newScene;
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		auto view = registry.view<CameraComponent>();
		for (auto [entity, camera] : view.each()) {
			if (!camera.fixedAspectRatio)
				camera.camera.SetViewportSize(width, height);
		}
	}

	void Scene::OnRuntimeStart()
	{
		//physics
		OnPhysics2DStart();

		//Scripting
		{
			ScriptEngine::OnRuntimeStart(this);
			auto view = registry.view<ScriptComponent>();
			for (auto [e, script] : view.each()) {
				Entity entity(e, this);
				ScriptEngine::OnCreateEntity(entity);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		//physics
		OnPhysics2DStop();

		//scripting
		{
			auto view = registry.view<ScriptComponent>();
			for (auto [e, script] : view.each()) {
				Entity entity(e, this);
				ScriptEngine::OnDestroyEntity(entity);
			}
		}
		ScriptEngine::OnRuntimeStop();
	}

	void Scene::OnRuntimeUpdate()
	{
		//Update rotation in transform component
		{
			auto view = registry.view<TransformComponent>();
			for (auto [entity, transform] : view.each()) {
				transform.UpdateRotation();
			}
		}

		if (!isPaused || framesToStep-- > 0)
		{

			//Scripting
			float dt = Application::GetDT();
			{
				auto view = registry.view<ScriptComponent>();
				for (auto [e, script] : view.each()) {
					Entity entity(e, this);
					ScriptEngine::OnUpdateEntity(entity, dt);
				}
			}

			//physics
			const uint32_t velocityIters = 6;
			const uint32_t positionIters = 2;
			physicsWorld->Step(Application::GetDT(), velocityIters, positionIters);

			for (auto& [paperID, body] : b2BodyMap)
			{
				Entity entity = paperID.ToEntity();
				auto& tc = entity.GetComponent<TransformComponent>();

				const auto& position = body->GetPosition();
				tc.position.x = position.x;
				tc.position.y = position.y;
				tc.rotation.z = glm::degrees(body->GetAngle());
			}

		}
		//get primary camera
		EntityCamera* entityCamera = nullptr;
		glm::mat4 cameraTransform;

		{
			auto view = registry.view<CameraComponent, TransformComponent>();
			for (auto [entity, camera, transform] : view.each()) {
				if (camera.primary)
				{
					entityCamera = &camera.camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		//Render
		if (entityCamera)
		{
			Renderer2D::BeginRender(*entityCamera, cameraTransform);
			Render();
			Renderer2D::EndRender();
		}
	}

	void Scene::OnSimulationStart()
	{
		OnPhysics2DStart();
	}

	void Scene::OnSimulationStop()
	{
		OnPhysics2DStop();
	}

	void Scene::OnSimulationUpdate(const Shr<EditorCamera>& camera)
	{
		//Update rotation in transform component
		{
			auto view = registry.view<TransformComponent>();
			for (auto [entity, transform] : view.each()) {
				transform.UpdateRotation();
			}
		}

		if (!isPaused || framesToStep-- > 0)
		{
			//physics
			const uint32_t velocityIters = 6;
			const uint32_t positionIters = 2;
			physicsWorld->Step(Application::GetDT(), velocityIters, positionIters);

			for (auto& [paperID, body] : b2BodyMap)
			{
				Entity entity = paperID.ToEntity();
				auto& tc = entity.GetComponent<TransformComponent>();

				const auto& position = body->GetPosition();
				tc.position.x = position.x;
				tc.position.y = position.y;
				tc.rotation.z = glm::degrees(body->GetAngle());
			}
		}

		//render
		Renderer2D::BeginRender(camera);
		Render();
		Renderer2D::EndRender();
	}

	void Scene::OnEditorUpdate(const Shr<EditorCamera>& camera)
	{
		//render
		EditorRender(camera);
	}

	void Scene::EditorRender(const Shr<EditorCamera>& camera)
	{
		Renderer2D::BeginRender(camera);
		Render();

		//camera icons
		{
			auto view = registry.view<TransformComponent, CameraComponent>();
			for (auto [entity, transform, line] : view.each())
			{
				EdgeRenderData data;
				data.texture = DataPool::GetTexture("resources/editor/world/camera_symbol.png", true);
				data.color = glm::vec4(1.0f);
				data.transform = transform.GetTransform() * glm::toMat4(glm::quat(glm::radians(glm::vec3(0.0f, -90.0f, 0.0f))));
				data.enity_id = (entity_id)entity;

				Renderer2D::DrawRectangle(data);
			}
		}

		Renderer2D::EndRender();
	}

	void Scene::Render()
	{
		//Sprites
		{
			auto view = registry.view<TransformComponent, SpriteComponent>();
			for (auto [entity, transform, sprite] : view.each()) {
				if (sprite.geometry == Geometry::CIRCLE)
				{
					CircleRenderData data;
					data.transform = transform.GetTransform();
					data.color = sprite.color;
					data.texture = sprite.texture;
					data.tilingFactor = sprite.tiling_factor;
					data.texCoords = sprite.tex_coords;
					data.coreIDToAlphaPixels = sprite.register_alpha_pixels_to_event;
					data.enity_id = (entity_id)entity;

					data.thickness = sprite.thickness;
					data.fade = sprite.fade;

					Renderer2D::DrawCircle(data);
				}
				else
				{
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
		}

		//lines
		{
			auto view = registry.view<TransformComponent, LineComponent>();
			for (auto [entity, transform, line] : view.each()) 
			{
				LineRenderData data;
				data.transform = transform.GetTransform();
				data.color = line.color;
				data.thickness = line.thickness;
				data.enity_id = (entity_id)entity;
				
				Renderer2D::DrawLine(data);
			}
		}

		//text
		{
			auto view = registry.view<TransformComponent, TextComponent>();
			for (auto [entity, transform, text] : view.each()) {

				TextRenderData data;
				data.transform = transform.GetTransform();
				data.color = text.color;
				data.text = text.text;
				data.font = text.font;
				data.coreIDToAlphaPixels = text.register_alpha_pixels_to_event;
				data.enity_id = (entity_id)entity;
				
				Renderer2D::DrawString(data);
			}
		}
	}

	void Scene::OnPhysics2DStart()
	{
		physicsWorld = new b2World({ 0.0f, -9.81f });

		auto view = registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity{ e, this };
			auto& tc = entity.GetComponent<TransformComponent>();
			auto& rb2dc = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.fixedRotation = rb2dc.fixedRotation;
			bodyDef.position = b2Vec2(tc.position.x, tc.position.y);
			bodyDef.angle = glm::radians(tc.rotation.z);
			bodyDef.type = (b2BodyType)rb2dc.type;
			
			b2Body* body = physicsWorld->CreateBody(&bodyDef);
			b2BodyMap[entity.GetPaperID()] = body;

			if (entity.HasComponent<Collider2DComponent>())
			{
				auto& c2dc = entity.GetComponent<Collider2DComponent>();

				if (c2dc.geometry == Geometry::NONE) continue;

				b2FixtureDef fixtureDef;
				fixtureDef.density = c2dc.density;
				fixtureDef.friction = c2dc.friction;
				fixtureDef.restitution = c2dc.bounciness;
				fixtureDef.restitutionThreshold = c2dc.bouncinessThreshold;

				if (c2dc.geometry == Geometry::CIRCLE)
				{
					b2CircleShape circleShape;
					circleShape.m_p.Set(c2dc.offset.x, c2dc.offset.y);
					circleShape.m_radius = c2dc.radius * tc.scale.x;
					fixtureDef.shape = &circleShape;
					body->CreateFixture(&fixtureDef);
				}
				else
				{
					b2PolygonShape polygonShape;
					polygonShape.SetAsBox(c2dc.size.x * tc.scale.x, c2dc.size.y * tc.scale.y);
					fixtureDef.shape = &polygonShape;
					body->CreateFixture(&fixtureDef);
				}
			}
			else
			{
				b2FixtureDef fixtureDef;
				b2PolygonShape polygonShape;
				polygonShape.SetAsBox(0.5f, 0.5f);
				fixtureDef.shape = &polygonShape;
				fixtureDef.density = 1.0f;
				fixtureDef.isSensor = true;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnPhysics2DStop()
	{
		delete physicsWorld;
		physicsWorld = nullptr;
		b2BodyMap.clear();
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntity(PaperID(), name);
	}

	Entity Scene::CreateEntity(const PaperID& id, const std::string& name)
	{
		Entity entity(registry.create(), id, name, this);
		entity_map[id] = entity;

		return entity;
	}

	Entity Scene::DuplicateEntity(Entity entity)
	{
		Entity dst = CreateEntity(entity.GetName());
		CopyComponentIfExists(AllComponents{}, dst, entity);
		return dst;
	}

	bool Scene::DestroyEntity(Entity entity)
	{
		if (!entity_map.contains(entity.GetPaperID())) return false;

		if (entity.HasComponent<ScriptComponent>())
			ScriptEngine::OnDestroyEntity(entity);

		entity_map.erase(entity.GetPaperID());
		registry.destroy(entity);
		return true;
	}

	Entity Scene::GetEntity(const PaperID& id)
	{
		//CORE_ASSERT(entity_map.contains(id), "Entity does not exists");
		if (!entity_map.contains(id))
			return {};

		return {entity_map.at(id), this};
	}

	Entity Scene::GetEntityByName(const std::string& name)
	{
		for (const auto& entt : entity_map | std::views::values)
		{
			Entity entity{ entt, this };
			if (entity.GetName() == name)
				return entity;
		}
		return Entity();
	}
}
