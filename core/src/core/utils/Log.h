#pragma once
#include "_Core.h"
#include "utility.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
namespace core {
	class CORE_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;

	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return clientLogger; }
	};
}

#ifdef BUILD_DEBUG

//Core Log
#define LOG_CORE_ERROR(...) ::core::Log::getCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_WARN(...)  ::core::Log::getCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_TRACE(...) ::core::Log::getCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_DEBUG(...) ::core::Log::getCoreLogger()->debug(__VA_ARGS__)

//Client Log
#define LOG_ERROR(...) ::core::Log::getClientLogger()->error(__VA_ARGS__)
#define LOG_WARN(...)  ::core::Log::getClientLogger()->warn(__VA_ARGS__)
#define LOG_TRACE(...) ::core::Log::getClientLogger()->info(__VA_ARGS__)
#define LOG_DEBUG(...) ::core::Log::getClientLogger()->debug(__VA_ARGS__)

#else 

//Core Log
#define LOG_CORE_ERROR(...)
#define LOG_CORE_WARN(...)
#define LOG_CORE_TRACE(...)
#define LOG_CORE_DEBUG(...)

//Client Log
#define LOG_ERROR(...)
#define LOG_WARN(...)
#define LOG_TRACE(...)
#define LOG_DEBUG(...)

#endif