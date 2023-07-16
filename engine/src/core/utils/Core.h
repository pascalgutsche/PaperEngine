#pragma once
#include "Engine.h"
#include "utility.h"
#include "generic/Object.h"

#if 0

namespace Paper
{
	class Core
	{
	public:
		static void Init();

		static entity_id RequestID(Object* object);
		static void DeleteID(entity_id id);

		static bool IsDeleted(entity_id id);
		static void AddToDelete(entity_id id);

		static Object* GetObjectByID(entity_id id);

		static entt::registry& GetRegistry();
	private:
		inline static std::unordered_map<entity_id, Object*> IDinUse;
		inline static std::vector<entity_id> deletedIDs;
		inline static entity_id leastIDavailable;
		inline static bool initialized = false;

		static entt::registry registry;
	};
}
#endif