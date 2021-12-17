//Program that shows what events the engine returns. The user should press with the keyboard, use the mouse and game controller
//
//April 16, 2021
//Lithio

#include "pwengine.hpp"

int main(int argc, char *argv[]){
	PWEngine *engine = new PWEngine;
	
	if(engine->initialize(800, 600, 30, "Primatype") == -1){
		return -1;
	}
	
	while(engine->is_quit() == false){
		engine->sleep_until_next_frame();
		engine->poll_all_events();
		engine->update_all();
		engine->render();
	}
	
	return 0;
}