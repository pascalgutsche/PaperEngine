#include "Engine.h"

#include "Core.h"

#if 0
namespace Paper
{
	void Core::Init()
	{
		CORE_ASSERT(!initialized, "Core already initialized");
		IDinUse[0] = nullptr;
		leastIDavailable = 1;
		initialized = true;

		registry = entt::registry();
	}

	entity_id Core::RequestID(Object* object)
	{
		CORE_ASSERT(initialized, "Core not initialized");
		uint32_t id = leastIDavailable;
		IDinUse[leastIDavailable] = object;
		leastIDavailable = IDinUse.size();
		return id;
	}

	void Core::DeleteID(entity_id id)
	{
		CORE_ASSERT(initialized, "Core not initialized");
		std::unordered_map<entity_id, Object*>::iterator it = IDinUse.find(id);
		if (it == IDinUse.end())
		{
			CORE_ASSERT(false, "ID does not exist");
			return;
		}
		IDinUse.erase(it);
		if (id < leastIDavailable) leastIDavailable = id;
	}

	bool Core::IsDeleted(entity_id id)
	{
		return std::find(deletedIDs.begin(), deletedIDs.end(), id) != deletedIDs.end();
	}

	void Core::AddToDelete(entity_id id)
	{
		deletedIDs.push_back(id);
	}

	//TODO: ERROR HERE
	Object* Core::GetObjectByID(entity_id id)
	{
		CORE_ASSERT(id > 0, "invalid ID");
		if (IDinUse.find(id) != IDinUse.end()) {
			return IDinUse.at(id);
		}
		CORE_ASSERT(false, "invalid ID");
		return nullptr;
	}

	entt::registry& Core::GetRegistry()
	{
		return registry;
	}
}
#endif