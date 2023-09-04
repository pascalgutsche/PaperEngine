#include "Engine.h"
#include "Timer.h"

namespace Paper
{
	Timer::Timer()
	{
		startPoint = std::chrono::high_resolution_clock::now();
	}

	float Timer::GetElapsedMillis()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startPoint).count() * 0.001;
	}

	float Timer::GetElapsedMicros()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startPoint).count() * 0.000001;
	}
}

