//Lithio (the developer's pseudonym
//February 19, 2022

#include "constants.hpp"
#include "map.hpp"
#include "map_transition.hpp"
#include "player.hpp"
#include "pwengine.hpp"

int main(int argc, char *argv[]){
	PWEngine *engine = new PWEngine();
	engine->initialize(kScreenWidth, kScreenHeight, kFrameRate, "Primatype 1");
	
	//create map and load map 0
	Map *map = new Map(engine);
	map->load(0);
	
	//initialize player in map with position 10, 33
	Player *player = new Player(engine);
	map->set_player(player);
	player->set_map(map);
	player->set_position(10.0f * kTileSize, 33.0f * kTileSize);
	
	//initialize map transition instance for moving through maps
	MapTransition *tr = new MapTransition(engine, player, map);
	map->set_map_transition(tr);
	
	//main game loop
	while(!engine->is_quit()){
		engine->poll_all_events();
		engine->update_all();
		engine->render();
		engine->sleep_until_next_frame();
	}
	
	//clear all memory
	delete engine;
	
	return 0;
}