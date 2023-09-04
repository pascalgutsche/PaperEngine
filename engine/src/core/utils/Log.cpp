#include "Engine.h"

#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Paper {
	std::shared_ptr<spdlog::logger> Log::coreLogger;
	std::shared_ptr<spdlog::logger> Log::editorLogger;
	std::shared_ptr<spdlog::logger> Log::scriptingLogger;

	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");

		coreLogger = spdlog::stdout_color_mt("CORE");
		coreLogger->set_level(spdlog::level::trace);
	
		editorLogger = spdlog::stdout_color_mt("EDITOR");
		editorLogger->set_level(spdlog::level::trace);

		scriptingLogger = spdlog::stdout_color_mt("SCRIPTING");
		scriptingLogger->set_level(spdlog::level::trace);
	}

	void Log::Shutdown()
	{
		coreLogger.reset();
		editorLogger.reset();
		scriptingLogger.reset();
		spdlog::drop_all();
	}
}