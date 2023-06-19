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

		template <typename T>
		std::vector<std::vector<T>> registry;
	};

	template <typename  T>
	void Registry::Add(T component)
	{
		static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");



	}
}
