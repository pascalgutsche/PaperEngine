#pragma once
#include "Engine.h"

#include "box2d/b2_world.h"

namespace Paper
{
	class Scene;

	class CollisionListener : public b2ContactListener
	{
	public:
		CollisionListener(Scene* scene);

		void BeginContact(b2Contact* contact) override;
		void EndContact(b2Contact* contact) override;

	private:
		Scene* scene;
	};
}