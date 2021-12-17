//Lithio
//January 8, 2021

//Player class

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL.h>

#include "pwengine.hpp"
#include "pwobject.hpp"
#include "space.hpp"

#include <string>

class Player: public PWObject{
public:
	Player(PWEngine *engine);
	~Player();
	
	void update();
private:
	Space position_;
	Space last_position_on_ground_;
	int direction_;
	float velocity_x_;
	float velocity_y_;
	bool is_flying_;
	
	//TODO - initialize at least
	int hp_;
	int max_hp_;
	int mp_;
	int max_mp_;
	int exp_;
	int max_exp_;
	int fly_;
	int max_fly_;
	
	std::string name_;
	
	//TODO - this will not be implemented yet.
	std::map<int, int> monster_kills_;
	
	//saved variables for NPC's to use (like browser cookies)
	std::map<int, int> npc_variables_;
	
	SDL_Texture *body_texture_;
	SDL_Texture *wing_texture_;
	SDL_Texture *shadow_texture_;
};

#endif
