//Lithio
//December 18, 2020

//compilation test file
#include <stdio.h>

#include "background.hpp"
#include "ball.hpp"
#include "pwengine.hpp"
#include "pwobject.hpp"

int main(int argc, char *argv[]){
	int num_objects;
	int i;
	
	printf("Enter how many objects to create: ");
	scanf("%d", &num_objects);
	
	PWEngine *engine = new PWEngine;
	
	if(engine->initialize(800, 600, 30, "Primatype") == -1){
		return -1;
	}
	
	//initialize a ball object
	PWObject *background = new Background(engine);
	PWObject *ball = nullptr;
	
	for(i = 0; i < num_objects; ++i){
		ball = new Ball(engine, 0, 5, 0);
	}
	
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
