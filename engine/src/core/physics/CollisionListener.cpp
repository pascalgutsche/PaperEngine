#include "Engine.h"
#include "CollisionListener.h"

#include <stack>

#include "scripting/ScriptEngine.h"

#include "box2d/b2_contact.h"

namespace Paper
{
	CollisionListener::CollisionListener(Scene* scene)
		: scene(scene)
	{
	}

	void CollisionListener::BeginContact(b2Contact* contact)
	{
		ScriptClass collision(ScriptCache::GetManagedClassFromName("Paper.Collision"));
		MonoObject* instance = collision.Instantiate();
		uint64_t uuidA = scene->GetEntity(contact->GetFixtureA()->GetBody()).GetPaperID().toUInt64();
		uint64_t uuidB = scene->GetEntity(contact->GetFixtureB()->GetBody()).GetPaperID().toUInt64();

		void* params[] =
		{
			&uuidA,
			&uuidB
		};

		if (const ManagedMethod* method = collision.GetMethod("BeginContact", 2))
			collision.InvokeMethod(instance, method, params);
	}

	void CollisionListener::EndContact(b2Contact* contact)
	{
		ScriptClass collision(ScriptCache::GetManagedClassFromName("Paper.Collision"));
		MonoObject* instance = collision.Instantiate();
		uint64_t uuidA = scene->GetEntity(contact->GetFixtureA()->GetBody()).GetPaperID().toUInt64();
		uint64_t uuidB = scene->GetEntity(contact->GetFixtureB()->GetBody()).GetPaperID().toUInt64();

		void* params[] =
		{
			&uuidA,
			&uuidB
		};

		if (const ManagedMethod* method = collision.GetMethod("EndContact", 2))
			collision.InvokeMethod(instance, method, params);
	}
}
