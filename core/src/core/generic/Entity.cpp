#include "_Core.h"

#include "generic/Entity.h"

#include "Application.h"
#include "generic/Component.h"
#include "generic/Transform.h"
#include "utils/DataPool.h"
#include "layer/Layer.h"

#include "event/Input.h"
#include "event/KeyCodes.h"

#include "imgui/ImGuiLayer.h"
#include "utils/Core.h"


namespace core {

	void Entity::StopComponentIndex(int32_t index)
	{
        if (index == -1)
        {
            renderComponent->OnStop();
            return;
        }
        components[index]->OnStop();
	}

	void Entity::DeleteComponentIndex(int32_t index)
	{
        if (index == -1)
        {
            delete renderComponent;
            return;
        }
        delete components[index];
	}

	void Entity::SetLayer(Layer* layer)
	{
        this->layer = layer;
	}

	RenderComponent* Entity::GetRenderComponent() const
	{
        return renderComponent;
	}

	void Entity::SetScene(Scene* scene)
	{
        this->scene = scene;
	}

	Entity::Entity(std::string name, const Transform& transform, ProjectionMode mode)
        : Object(name, transform), mode(mode)
	{
        renderComponent = nullptr;
    }


    Entity::~Entity()
    {
        this->deleted = true;
        if (layer) {
            std::vector<Entity*>::iterator it = std::find(layer->GetEntitys().begin(), layer->GetEntitys().end(), this);
            if (it != layer->GetEntitys().end())
            {
                layer->GetEntitys().erase(it);
            }
        }
		if (scene) {
            std::vector<Entity*>::iterator it = std::find(scene->GetEntitys().begin(), scene->GetEntitys().end(), this);
            if (it != scene->GetEntitys().end())
            {
                scene->GetEntitys().erase(it);
            }
        }
        const int componentSize = components.size();
        for (int i = 0; i < componentSize; i++)
        {
            if (components.size() <= 0) break;

            if (running)
                components[0]->OnStop();
            delete components[0];
            components.erase(components.begin());
        }
        if (renderComponent)
        {
            if (running)
                renderComponent->OnStop();
            delete renderComponent;
        }
    }


    bool Entity::AddComponent(Component* component)
	{
        if (dynamic_cast<RenderComponent*>(component) != nullptr)
        {
            if (renderComponent != nullptr) return false;
            renderComponent = dynamic_cast<RenderComponent*>(component);
            renderComponent->gameObject = this;
            return true;
        }
        for (const auto i : components) 
        {
            if (i == component) 
            {
                return false;
            }
        }
        components.push_back(component);
        component->gameObject = this;
        return true;
    }

    void Entity::Update()
	{
        core_id id = coreID;
        // update gameObject, in order to display moving changes
        for (auto component : components) {
            if (component) {
                component->OnUpdate();
            }
            if (Core::IsDeleted(id)) return;
        }
		transform.Update();
    }

    void Entity::Start()
	{
        // start all components
        running = true;
        for (auto component : components) {
            component->OnStart();
        }
    }

    void Entity::Stop()
    {
        running = false;
        for (auto component : components) 
        {
            component->OnStop();
        }
    }

    void Entity::DeleteComponents()
	{
        // delete all components
        for (auto comp : components)
        {
            if (running) 
				comp->OnStop();
            delete comp;
            comp = nullptr;
        }
        components.clear();
    }

    Entity* Entity::AddTag(std::string tag)
    {
        std::transform(tag.begin(), tag.end(), tag.begin(), ::toupper);
        if (std::find(tagList.begin(), tagList.end(), tag) != tagList.end())
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
            std::transform(tag.begin(), tag.end(), tag.begin(), ::toupper);
            AddTag(tag);
        }
        return this;
    }

    bool Entity::RemoveTag(std::string tag)
    {
        std::transform(tag.begin(), tag.end(), tag.begin(), ::toupper);
        std::vector<std::string>::iterator it = std::find(tagList.begin(), tagList.end(), tag);
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
        std::transform(tag.begin(), tag.end(), tag.begin(), ::toupper);
        std::vector<std::string>::iterator it = std::find(tagList.begin(), tagList.end(), tag);
        return it != tagList.end();
    }


    void Entity::OnEvent(Event& event)
    {
	    for (auto* component : components)
	    {
            component->OnEvent(event);
	    }
    }


	void Entity::Imgui(float dt) {
		ImGui::Text("Transform:");
        float speed = 0.5f;
        if (Input::IsKeyPressed(KEY_LEFT_SHIFT) && Input::IsKeyPressed(KEY_LEFT_CONTROL))
            speed = 0.001f;
        else if (Input::IsKeyPressed(KEY_LEFT_SHIFT))
            speed = 0.01f;
        
		ImGui::DragFloat(std::string("X:").c_str(), &this->transform.position.x, speed);
		ImGui::DragFloat(std::string("Y:").c_str(), &this->transform.position.y, speed);
		ImGui::DragFloat(std::string("Z:").c_str(), &this->transform.position.z, speed);
		ImGui::DragFloat(std::string("Scale X:").c_str(), &this->transform.scale.x, speed);
		ImGui::DragFloat(std::string("Scale Y:").c_str(), &this->transform.scale.y, speed);
		ImGui::DragFloat(std::string("Scale Z:").c_str(), &this->transform.scale.z, speed);
        float rot[]{ this->transform.rotation.x, this->transform.rotation.y, this->transform.rotation.z };
        ImGui::DragFloat3(std::string("Rotation:").c_str(), rot, speed);
        this->transform.rotation = glm::vec3(rot[0], rot[1], rot[2]);
        

		for (auto component : components) {
		    component->OnImgui(dt);
		}
    }

    

    

}