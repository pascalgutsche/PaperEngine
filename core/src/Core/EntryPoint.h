#pragma once

#ifdef CORE_PLATFORM_WINDOWS

extern core::Application* core::createApplication();

int main(int argc, char ** argv) {
	auto app = core::createApplication();
	app->run();
	delete app;
}

#endif