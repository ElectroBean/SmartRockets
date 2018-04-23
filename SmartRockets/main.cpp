#include "SmartRocketsApp.h"

int main() {
	
	// allocation
	auto app = new SmartRocketsApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}