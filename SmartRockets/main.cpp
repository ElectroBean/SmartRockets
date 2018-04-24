#include "SmartRocketsApp.h"
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 

int main() {
	
	// allocation
	auto app = new SmartRocketsApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// deallocation
	delete app;

	return 0;
}