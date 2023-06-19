#pragma once
#include "Engine.h"

#include "Component.h"

#include <type_traits>

namespace engine
{
	class Registry
	{
	public:
		Registry();
		~Registry();

		template<class T>
		void Add(T component);

	private:

		std::vector<std::vector<Component>> registry;
	};

	template <typename  T>
	void Registry::Add(T component)
	{
		static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

		for (auto& vec : registry)
		{
			if (dynamic_cast<T*>(&vec[0]) == nullptr) continue;
			vec.push_back(component);
			return;
		}

		registry.resize(registry.size() + 1, std::vector<Component>());

	}
}
