//Lithio
//February 21, 2022

#ifndef MAP_HPP
#define MAP_HPP

#include "pwengine.hpp"
#include "pwobject.hpp"
#include "space.hpp"
#include <vector>
#include <map>

class MapTransition;
//class Mob;
class Player;

struct Portal{
	Space space;
	PWVec2 dest_pos;
	int dest_map;
};

class Map: public PWObject{
public:
	Map(PWEngine *engine);
	~Map();
	
	//this gets called every game tick
	virtual void update();
	
	void load(int map_id);
	void set_player(Player *p);
	void set_map_transition(MapTransition *mt);
	
	int get_width();
	int get_height();
	const int *get_tiles();
private:
	int width_;
	int height_;
	int *tiles_;
	//std::vector<Mob*> mobs_;
	std::vector<Portal> portals_;
	std::map<int, PWSprite*> sprites_;
	Player *player_;
	MapTransition *map_transition_;
};

#endif
