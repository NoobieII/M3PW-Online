//Lithio
//January 8, 2021

//Player class

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL.h>

#include "pwengine.hpp"
#include "pwobject.hpp"
#include "space.hpp"

class Player: public PWObject{
public:
	Player(PWEngine *engine);
	~Player();
	
	void update();
private:
	Space position_;
	int direction_;
	float velocity_x_;
	float velocity_y_;
	bool is_flying_;
	
	SDL_Texture *body_texture_;
	SDL_Texture *wing_texture_;
	SDL_Texture *shadow_texture_;
};

#endif
