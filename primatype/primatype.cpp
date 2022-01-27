//Lithio (the developer's pseudonym)
//January 8, 2021

#include "pwbutton.hpp"
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
	PWObject *player2 = new Player(engine);
	
	//test, creating blue background
	PWSprite *blue_bg = engine->load_sprite(
		"sprites/blue_bg.png",
		800,
		600,
		0,
		0,
		1,
		1
	);
	
	//test, create button
	PWButton *log_in_button = new PWButton(
		490,
		290,
		80,
		30,
		2,
		engine->load_sprite("sprites/log_in_button.png", 80, 30, 0, 0, 1, 1),
		engine->load_sprite("sprites/log_in_button_pushed.png", 80, 30, 0, 0, 1, 1),\
		"log_in_button",
		engine
	);
	
	//main loop
	while(engine->is_quit() == false){
		engine->sleep_until_next_frame();
		engine->poll_all_events();
		engine->update_all();
		
		engine->draw(blue_bg, 0, 0, 9);
		
		//test drawing text
		//engine->draw_str_wrapped("Create Account", 0, 0, 800, 600, 100, 0);
		//engine->draw_str_wrapped("Create Account", 1, 0, 800, 600, 100, 0);
		
		//
		
		
		engine->render();
	}
	printf("deleting engine...\n");
	delete engine;
	
	printf("Program terminates.\n");
	return 0;
}
