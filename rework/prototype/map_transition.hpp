//Lithio
//March 7, 2022

//Map transition class

#ifndef MAP_TRANSITION_HPP
#define MAP_TRANSITION_HPP

#include "pwobject.hpp"
#include "pwvec2.hpp"

class Map;
class Player;
struct Portal;

class MapTransition: public PWObject{
public:
	MapTransition(PWEngine *engine, Player *p, Map *m);
	~MapTransition();
	
	virtual void update();
	
	void move_player_to_map(Portal *p);
private:
	int state_;
	Player *player_;
	Map *map_;
	PWVec2 dest_pos_;
	int dest_map_;
	
	//for map transition effect
	PWSprite *sprites_[8];
};

#endif
