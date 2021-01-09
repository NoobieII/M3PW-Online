//Lithio
//January 8, 2021

#include "pwengine.hpp"
#include "player.hpp"

int main(int argc, char *argv[]){
	//create the engine
	PWEngine *engine = new PWEngine;
	
	if(engine->initialize(800, 600, 30, "Primatype") == -1){
		return -1;
	}
	
	//create a player
	PWObject *player = new Player(engine);
	
	//main loop
	while(engine->is_quit() == false){
		engine->sleep_until_next_frame();
		engine->poll_all_events();
		engine->update_all();
		engine->render();
	}
	
	delete engine;
	
	printf("Program terminates.\n");
	return 0;
}
