#pragma once

#ifdef CORE_PLATFORM_WINDOWS
	#ifdef CORE_BUILD_DLL
		#define CORE_API //__declspec(dllexport)
	#else
		#define CORE_API //__declspec(dllimport)
	#endif
#else
	#error This Engine only supports Windows!
#endif

#ifdef CORE_ENABLE_ASSERTS
#define ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define CORE_ASSERT(x, ...) { if(!(x)) { LOG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define HZ_ASSERT(x, ...)
#define HZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace core {

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

}