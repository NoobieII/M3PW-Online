//Lithio
//December 27, 2020

//ball has size, layer number and color
//game object that moves up and down, with sprite
//for testing the engine

#ifndef BALL_HPP
#define BALL_HPP

#include <SDL.h>

#include "pwobject.hpp"

class Ball: public PWObject{
public:
	//spawn a ball in the centre (400, 300) and destroy when far enough
	//from the centre and create a new ball. make a random velocity
	Ball(PWEngine *engine, int size, int layer_number, int color);
	~Ball();
	
	//move the ball
	void update();
private:
	int vx_;
	int vy_;
	int x_;
	int y_;
	int layer_number_;
	SDL_Texture *sprite_;
};

#endif
