//Lithio
//December 27, 2020

#include "background.hpp"
#include "pwengine.hpp"

Background::Background(PWEngine *engine):
PWObject("background", engine, true),
sprite_(nullptr)
{
	sprite_ = engine_->load_image("test-images/background01.jpg");
}

Background::~Background(){
	SDL_DestroyTexture(sprite_);
}

//draw the sprite on layer 10
void Background::update(){
	engine_->draw(sprite_, NULL, NULL, 9);
	
	if(engine_->is_key_pressed("T")){
		destroy();
	}
}
