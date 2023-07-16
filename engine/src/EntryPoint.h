#pragma once

#ifdef CORE_PLATFORM_WINDOWS
#ifdef INCLUDE_MAIN


extern Paper::Application* Paper::CreateApplication();


int main(int argc, char ** argv) 
{
	do
	{
		auto app = Paper::CreateApplication();
		app->Run();
		delete app;
	} while (Application::restart);
}

#endif
#endif