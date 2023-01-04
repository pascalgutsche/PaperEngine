#pragma once


#include "utility.h"

namespace core {

	class CORE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void run();
	};
	
	//defined by client
	Application* createApplication();
}

