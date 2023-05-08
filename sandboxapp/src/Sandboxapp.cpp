#include "_Game.h"

#define INCLUDE_MAIN
#include "EntryPoint.h"

class Sandboxapp : public Application {
public:
	Sandboxapp() {
		
	}
	
	~Sandboxapp() override {
		
	}

	void Init() override {
		// Uncomment and set the start scene (pointer)
		//Application::ChangeScene(); 
	}

};

Application* core::CreateApplication() {
	return new Sandboxapp();
}
