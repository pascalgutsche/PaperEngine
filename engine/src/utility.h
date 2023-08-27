#pragma once
#include "Engine.h"

//#ifdef CORE_ENABLE_ASSERTS
//#define ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
//#define CORE_ASSERT(x, ...) { if(!(x)) { LOG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
//#else
//#define ASSERT(x, ...)
//#define CORE_ASSERT(x, ...)
//#endif

#ifdef CORE_ENABLE_ASSERTS

#define CORE_ASSERT_MESSAGE_INTERNAL(...)  LOG_CORE_CRITICAL("Assertion Failed" __VA_OPT__(,) __VA_ARGS__)
#define ASSERT_MESSAGE_INTERNAL(...)  LOG_CRITICAL("Assertion Failed" __VA_OPT__(,) __VA_ARGS__)

#define CORE_ASSERT(condition, ...) { if(!(condition)) { CORE_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); __debugbreak(); } }
#define ASSERT(condition, ...) { if(!(condition)) { ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); __debugbreak(); } }

#else
#define CORE_ASSERT(condition, ...)
#define ASSERT(condition, ...)
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define BIND_BUTTON_EVENT_FN(x) std::bind(&x, this)

namespace Paper {

	template<typename T>
	using Shr = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Shr<T> MakeShr(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> MakeScoped(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	using entity_id = uint32_t;
}
