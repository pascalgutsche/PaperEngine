#pragma once
#include "_Core.h"
#include "utility.h"

namespace core {

	
	class FrameBuffer
	{
	private:
		unsigned int fboID;

	public:
		FrameBuffer();
		~FrameBuffer();

		void invalidate();
		void bind();
		void unbind();
	};
}


