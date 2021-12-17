//Lithio
//May 4, 2021

//Statistics class holds player information
//TODO: integrate the statistics, npc data and saved data in one class?

#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <map>

class Player;

class Statistics{
public:
	Statistics(Player *player);
	~Statistics();
	
	//get currency count - yes, puffs is the name of the currency
	int get_puffs();
	
	//get the number of items the player is carrying, by item id
	int get_item_count(int id);
	
	//get the number of monsters the player has killed, by monster id
	int get_kill_count(int id);
private:
	Player *player_;
};

#endif
