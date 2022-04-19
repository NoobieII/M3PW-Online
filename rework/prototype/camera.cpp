//Lithio
//February 27, 2022

#include "camera.hpp"
#include "constants.hpp"
#include "map.hpp"
#include "player.hpp"

namespace camera{
	//calculate the 2D offset of things on the map screen
	PWVec2 offset(Player *p){
		PWVec2 v;
		PWVec2 midpoint = p->get_space().midpoint();
		Map *map = p->get_map();
		
		if(map->get_width() * kTileSize < kScreenWidth){
			v.x = (float)(kScreenWidth - map->get_width() * kTileSize) / 2.0f;
		}
		else if((int)(midpoint.x) < kScreenWidth / 2){
			v.x = 0.0f;
		}
		else if((int)(midpoint.x) > map->get_width() * kTileSize - kScreenWidth / 2){
			v.x = (float)(map->get_width() * kTileSize - kScreenWidth);
		}
		else{
			v.x = midpoint.x - (float)(kScreenWidth) / 2.0f;
		}
		
		
		if(map->get_height() * kTileSize < kScreenHeight){
			v.y = (float)(kScreenHeight - map->get_height() * kTileSize) / 2.0f;
		}
		else if((int)(midpoint.y) < kScreenHeight / 2){
			v.y = 0.0f;
		}
		else if((int)(midpoint.y) > map->get_height() * kTileSize - kScreenHeight / 2){
			v.y = (float)(map->get_height() * kTileSize - kScreenHeight);
		}
		else{
			v.y = midpoint.y - (float)(kScreenHeight) / 2.0f;
		}
		
		return -v;
	}
	
}
