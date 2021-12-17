//Lithio (the developer's pseudonym)
//June 22, 2021

#include "pwtextbox.hpp"
#include "pwengine.hpp"
#include "player.hpp"

int main(int argc, char *argv[]){
	//create the engine
	PWEngine *engine = new PWEngine;
	
	if(engine->initialize(800, 600, 30, "Primatype") == -1){
		return -1;
	}
	
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
	
	//test, create textbox
	PWSprite *textbox_underlay = engine->load_sprite(
		"sprites/test_text_underlay.png",
		100,
		12,
		0,
		0,
		1,
		1
	);
	
	PWTextbox textbox(
		500,
		300,
		100,
		100,
		150,
		1,
		false,
		true,
		textbox_underlay,
		"test_textbox",
		engine
	);
	textbox.deactivate();
	
	PWTextbox textbox2(
		0,
		0,
		800,
		600,
		1000,
		0,
		false,
		false,
		nullptr,
		"test_textbox2",
		engine
	);
	
	//completely separate tests, testing engine functionality.
	printf("test_textbox2 has object id %d\n", textbox2.get_id());
	
	PWTextbox *test = (PWTextbox*)(engine->get_object("test_textbox2"));
	
	if(!test){
		printf("failed to retrieve test_textbox2 by object name\n");
	}
	
	test = (PWTextbox*)(engine->get_object_by_id(textbox2.get_id()));
	if(!test){
		printf("failed to retrieve test_textbox2 by object name\n");
	}
	
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
	
	delete engine;
	
	delete blue_bg;
	delete textbox_underlay;
	
	printf("Program terminates.\n");
	return 0;
}
