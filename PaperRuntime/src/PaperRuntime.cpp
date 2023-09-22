#include "Runtime.h"

#define INCLUDE_MAIN
#include "EntryPoint.h"
#include "runtime/RuntimeLayer.h"

#include "nfd.hpp"

class PaperRuntime : public Application {
public:
	PaperRuntime()
		: Application(WindowProps("PaperEditor", 2560, 1200))
	{
		NFD::Init();
	}
	
	~PaperRuntime() override
	{
		NFD::Quit();
	}

	void Init() override {
		AddLayer(new RuntimeLayer());
	}
};

Application* Paper::CreateApplication() {
	return new PaperRuntime();
}