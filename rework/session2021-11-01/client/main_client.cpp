//Lithio
//October 08, 2021
//
//Client main function

#include <cstdio>

#include "pwengine.hpp"
#include "random/random.hpp"

int main(int argc, char *argv[]){
	//initialize random number generator
	pw_rng::init();
	
	//create the engine
	PWEngine *engine = new PWEngine;
	
	//initialize with screen width, height, framerate, window name
	if(engine->initialize(80, 30, 3, "Primatype") == -1){
		printf("could not initialize engine\n");
		return -1;
	}
	if(engine->connect("10.0.0.173", 16555) == -1){
		printf("could not connect to server");
		return -1;
	}
	
	int i = 0;
	
	//run until the X button is pressed
	while(i < 19){
		engine->sleep_until_next_frame();
		engine->poll_all_events();
		engine->update_all();
		
		++i;
	}
	
	engine->disconnect();
	delete engine;
	
	printf("Program terminates.\n");
	return 0;
}
