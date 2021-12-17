//test animated sprite program
//Lithio
//April 18, 2021

#include "pwengine.hpp"
#include "test_dragon.hpp"

#include <cstdio>

int main(int argc, char *argv[]){
	PWEngine *engine;
	TestDragon *dragon;
	
	engine = new PWEngine;
	
	if(engine->initialize(320, 240, 30, "Test Object") == -1){
		printf("Could not initialize engine.\n");
		return -1;
	}
	
	dragon = new TestDragon("dragon", engine);
	
	while(engine->is_quit() == false){
		engine->sleep_until_next_frame();
		engine->poll_all_events();
		engine->update_all();
		engine->render();
	}
	
	//clean up
	delete engine;
	
	printf("Program closes cleanly.\n");
	
	return 0;
}