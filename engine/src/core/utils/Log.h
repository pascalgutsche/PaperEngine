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
		static std::shared_ptr<spdlog::logger> editorLogger;
		static std::shared_ptr<spdlog::logger> scriptingLogger;

	public:
		static void Init();
		static void Shutdown();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetEditorLogger() { return editorLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetScriptingLogger() { return scriptingLogger; }
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
#define LOG_CRITICAL(...) ::Paper::Log::GetEditorLogger()->critical(__VA_ARGS__)
#define LOG_ERROR(...)	  ::Paper::Log::GetEditorLogger()->error(__VA_ARGS__)
#define LOG_WARN(...)	  ::Paper::Log::GetEditorLogger()->warn(__VA_ARGS__)
#define LOG_TRACE(...)	  ::Paper::Log::GetEditorLogger()->info(__VA_ARGS__)
#define LOG_DEBUG(...)	  ::Paper::Log::GetEditorLogger()->debug(__VA_ARGS__)

//Scripting Log
#define LOG_SCR_CRITICAL(...) ::Paper::Log::GetScriptingLogger()->critical(__VA_ARGS__)
#define LOG_SCR_ERROR(...)	  ::Paper::Log::GetScriptingLogger()->error(__VA_ARGS__)
#define LOG_SCR_WARN(...)	  ::Paper::Log::GetScriptingLogger()->warn(__VA_ARGS__)
#define LOG_SCR_TRACE(...)	  ::Paper::Log::GetScriptingLogger()->info(__VA_ARGS__)
#define LOG_SCR_DEBUG(...)	  ::Paper::Log::GetScriptingLogger()->debug(__VA_ARGS__)

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

//Scripting Log
#define LOG_SCR_CRITICAL(...)
#define LOG_SCR_ERROR(...)
#define LOG_SCR_WARN(...)
#define LOG_SCR_TRACE(...)
#define LOG_SCR_DEBUG(...)

#endif