//Lithio
//February 21, 2022

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "space.hpp"

class Map;
struct PWSprite;

class Player: public PWObject{
public:
	Player(PWEngine *engine);
	~Player();
	
	//to be called every game tick
	virtual void update();
	
	const Space &get_space();
	void set_position(int x, int y);
	void set_position(PWVec2 pos);
	
	void set_map(Map *m);
	Map *get_map();
private:
	Space space_;
	Map *map_;
	
	PWSprite *sprite_;
	PWSprite *sprite_up_;
	PWSprite *sprite_down_;
	PWSprite *sprite_left_;
	PWSprite *sprite_right_;
};

#endif
