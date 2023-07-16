#pragma once
#include "Engine.h"
#include "utility.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
namespace Paper {
	class Log
	{
	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;

	public:
		static void Init();
		static void Shutdown();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return clientLogger; }
	};
}

#ifdef BUILD_DEBUG

//Core Log
#define LOG_CORE_CRITICAL(...) ::Paper::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define LOG_CORE_ERROR(...)	   ::Paper::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_WARN(...)	   ::Paper::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_TRACE(...)	   ::Paper::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_DEBUG(...)	   ::Paper::Log::GetCoreLogger()->debug(__VA_ARGS__)

//Client Log
#define LOG_CRITICAL(...) ::Paper::Log::GetClientLogger()->critical(__VA_ARGS__)
#define LOG_ERROR(...)	  ::Paper::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_WARN(...)	  ::Paper::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_TRACE(...)	  ::Paper::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_DEBUG(...)	  ::Paper::Log::GetClientLogger()->debug(__VA_ARGS__)

#else 

//Core Log
#define LOG_CORE_CRITICAL(...)
#define LOG_CORE_ERROR(...)
#define LOG_CORE_WARN(...)
#define LOG_CORE_TRACE(...)
#define LOG_CORE_DEBUG(...)

//Client Log
#define LOG_CRITICAL(...)
#define LOG_ERROR(...)
#define LOG_WARN(...)
#define LOG_TRACE(...)
#define LOG_DEBUG(...)

#endif