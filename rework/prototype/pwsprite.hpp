//Lithio
//March 9, 2021

#ifndef PWSPRITE_HPP
#define PWSPRITE_HPP

#include <SDL.h>

//graphics, not ASCII
struct PWSprite{
	//top left position of the sprite in the texture
	int x_origin;
	int y_origin;
	
	//frame width and height
	int w_frame;
	int h_frame;
	
	//for use in an animated sprite
	//number of frames (for use in an animated sprite)
	int num_frames;
	
	//starting point in time
	int start_time;
	
	//period of each frame
	int frame_length;
	
	//refer to this texture (full size image)
	SDL_Texture *texture;
};

#endif
