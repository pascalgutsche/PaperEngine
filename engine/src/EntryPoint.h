#pragma once

#ifdef CORE_PLATFORM_WINDOWS
#ifdef INCLUDE_MAIN


extern ppr::Application* ppr::CreateApplication();

int main(int argc, char ** argv) 
{
	auto app = ppr::CreateApplication();
	app->Run();
	delete app;
}

#endif
#endif