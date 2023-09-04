#pragma once

namespace Paper
{
	class Timer
	{
	public:
		Timer();

		float GetElapsedMillis();
		float GetElapsedMicros();

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> startPoint;
	};
}

