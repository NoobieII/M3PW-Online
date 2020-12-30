//Lithio
//December 18, 2020

//compilation test file
#include <stdio.h>

#include "background.hpp"
#include "ball.hpp"
#include "pwengine.hpp"
#include "pwobject.hpp"

int main(int argc, char *argv[]){
	int i;
	PWEngine *engine = new PWEngine;
	
	if(engine->initialize(800, 600, 30, "Primatype") == -1){
		return -1;
	}
	
	//initialize a ball object
	PWObject *ball = new Ball(engine, 0, 5, 0);
	PWObject *background = new Background(engine);
	
	for(i = 0; i < 300; ++i){
		engine->sleep_until_next_frame();
		engine->poll_all_events();
		engine->update_all();
		engine->render();
	}
	ball->destroy();
	background->destroy();
	
	delete engine;
	
	printf("program terminates.\n");
	return 0;
}
