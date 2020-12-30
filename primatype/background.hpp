//Lithio
//December 27, 2020

#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <SDL.h>

#include "pwobject.hpp"

//Background class
class Background: public PWObject{
public:
	Background(PWEngine *engine);
	~Background();
	
	//render the sprite on layer 10
	void update();
private:
	SDL_Texture *sprite_;
};

#endif
