#include "_Core.h"

#include "layer/Layer.h"
#include "generic/Application.h"

namespace core
{
	void Layer::attach(bool add_to_renderer)
	{
		attached = true;
		for (GameObject* game_object : game_objects)
		{
			game_object->start();
		}
		OnAttach();
		if (add_to_renderer)
			Application::GetCurrentScene()->GetRenderer().add(this, Application::GetLayerStack().GetPlace(this));
	}

	void Layer::detach()
	{
		attached = false;
		OnDetach();
	}

	void Layer::AddGameObjectToLayer(GameObject* game_object)
	{
		if (attached) {
			game_objects.push_back(game_object);
			game_object->start();
		}
		else {
			game_objects.push_back(game_object);
		}
	}

}
