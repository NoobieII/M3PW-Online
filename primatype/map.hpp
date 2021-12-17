//Lithio
//May 23, 2021
//Map class

#ifndef MAP_HPP
#define MAP_HPP

#include "pwobject.hpp"

#include <map>

class Player;

class Map: public PWObject{
public:
	//constructor
	Map(PWEngine *engine);
	
	//destructor
	~Map();
private:
	int id_;
	int width_;
	int height_;
	
	//TODO
	//players
	std::map<int, Player*> players_;
	//npc's
}

#endif