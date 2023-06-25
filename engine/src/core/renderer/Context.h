#pragma once
#include "Engine.h"
#include "utility.h"

namespace ppr
{
	class Context
	{
	public:
		virtual ~Context() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void SwapBuffer() = 0;

		static Shr<Context> CreateContext(void* window);
	};
}

