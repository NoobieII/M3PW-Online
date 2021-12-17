//Lithio
//October 08, 2021
//
//Client main function

#include <cstdio>

#include "engine.hpp"
#include "random/random.hpp"

int main(int argc, char *argv[]){
	//initialize random number generator
	random::init_rng();
	
	//create the engine
	Engine *engine = new Engine;
	
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
	while(i < 10){
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
