#pragma once

#ifdef CORE_PLATFORM_WINDOWS
#ifdef INCLUDE_MAIN

extern engine::Application* engine::CreateApplication();

int main(int argc, char ** argv) 
{
	auto app = engine::CreateApplication();
	app->Run();
	delete app;
}

#endif
#endif