#include "_Game.h"

#define INCLUDE_MAIN
#include "EntryPoint.h"

class [Game] : public Application {
public:
	[Game]() {
		
	}
	
	~[Game]() override {
		
	}

	void Init() override {
		// Uncomment and set the start scene (pointer)
		//Application::ChangeScene(); 
	}

};

Application* core::CreateApplication() {
	return new [Game]();
}