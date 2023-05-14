#include "Engine.h"

#include "layer/Layer.h"
#include "event/Event.h"
#include "renderer/Renderer2D.h"
#include "ui/Button.h"
#include "ui/UIObject.h"

namespace engine
{

	Layer::~Layer()
	{
		const int layerSize = gameObjects.size();
		for (int i = 0; i < layerSize; i++)
		{
			if (gameObjects.size() > 0)
				delete gameObjects[0];
		}
		gameObjects.clear();
	}

	void Layer::Attach()
	{
		attached = true;
		for (Entity* game_object : gameObjects)
		{
			game_object->Start();
		}
		OnAttach();
	}

	void Layer::Detach()
	{
		attached = false;
		for (Entity* game_object : gameObjects)
		{
			game_object->Stop();
		}
		OnDetach();
	}

	void Layer::LayerEvent(Event& event)
	{
		for (Entity* gm : gameObjects)
		{
			if (!gm->IsRunning()) continue;
			if (!event.handled)
			{
				gm->OnEvent(event);
			}
		}
		if (!event.handled)
		{
			OnEvent(event);
		}
	}


	void Layer::AddEntityToLayer(Entity* gameObject)
	{
		if (attached) {
			gameObjects.push_back(gameObject);
			gameObject->Start();
		}
		else {
			gameObjects.push_back(gameObject);
		}
		gameObject->SetLayer(this);
	}

	std::vector<Entity*> Layer::GetEntitysByTag(std::initializer_list<std::string> tags)
	{
		std::vector<Entity*> gos;
		for (int i = 0; i < gameObjects.size(); i++)
		{
			for (std::string tag : tags)
			{
				if (gameObjects[i]->HasTag(tag))
				{
					gos.push_back(gameObjects[i]);
					break;
				}
			}
		}
		return gos;
	}

	std::vector<Entity*> Layer::GetEntitysByTag(std::string tag)
	{
		std::vector<Entity*> gos;
		for (int i = 0; i < gameObjects.size(); i++)
		{
			if (gameObjects[i]->HasTag(tag))
			{
				gos.push_back(gameObjects[i]);
			}
		}
		return gos;
	}

	void Layer::AddUIObject(UIObject* object, ProjectionMode mode)
	{
		object->globalPos = object->transform.position;
		object->globalScale = object->transform.scale;
		object->mode = mode;
		uiObjects.push_back(object);
	}

	void Layer::RemoveUIObject()
	{
		uiObjects.clear();
	}

	void Layer::RemoveUIObject(uint32_t index)
	{
		if (index >= uiObjects.size()) return;
		uiObjects.erase(uiObjects.begin() + index);
	}

	void Layer::RemoveUIObject(UIObject* object)
	{
		std::vector<UIObject*>::iterator it = std::find(uiObjects.begin(), uiObjects.end(), object);
		if (it == uiObjects.end()) return;
		uiObjects.erase(it);
	}

	std::vector<UIObject*>& Layer::GetUIObjects()
	{
		return uiObjects;
	}

	void Layer::RenderUI()
	{
		for (UIObject* object : uiObjects) {
			RenderObject(object);
		}
	}

	void Layer::RenderObject(UIObject* object)
	{
		object->CalculateGlobalCoords();
		object->RenderObject();
		Renderer2D::NextBatch(ALL);
		for (UIObject* child : object->GetChildObjects())
		{
			if (child != object)
				child->mode = object->mode;
				RenderObject(child);
		}
	}

	
}
