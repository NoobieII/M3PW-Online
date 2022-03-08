//Lithio
//February 21, 2022

#include <cstdio>
#include <cstring>
#include "camera.hpp"
#include "constants.hpp"
#include "map.hpp"
#include "map_transition.hpp"
#include "player.hpp"

Map::Map(PWEngine *engine):
PWObject("map", engine, true),
width_(0),
height_(0),
tiles_(nullptr),
//mobs_(),
portals_(),
sprites_(),
player_(nullptr),
map_transition_(nullptr)
{
}

Map::~Map(){
	int i;
	
	if(tiles_){
		delete[] tiles_;
	}
	
	//the map created the mob instances so it should delete them
	//for(i = 0; i < mobs_.size(); ++i){
	//	delete mobs_[i];
	//}
	
	//no need to delete the player
	
	//delete the sprites
	for(auto &it : sprites_){
		if(it.second){
			delete it.second;
		}
	}
}

void Map::update(){
	//draw the map on screen
	int i, j;
	PWVec2 offset;
	PWSprite *s;
	
	//TODO: check collision with portals
	for(i = 0; i < portals_.size(); ++i){
		if(portals_[i].space.is_colliding(player_->get_space())){
			map_transition_->move_player_to_map(&portals_[i]);
		}
	}
	
	//draw the map
	offset = camera::offset(player_);
	
	for(i = 0; i < height_; ++i){
		for(j = 0; j < width_; ++j){
			s = sprites_[tiles_[i * width_ + j] / 2];
			
			if(!s){
				//load the sprite
				s = engine_->load_sprite("sprites.png", 64, 64, tiles_[i * width_ + j] / 2 * 64, 64, 1, 1);
				sprites_[tiles_[i * width_ + j] / 2] = s;
			}
			
			if(tiles_[i * width_ + j] % 2 == 0){
				engine_->draw(s, j * 64 + (int)(offset.x), i * 64 + (int)(offset.y), 3);
			}
			else {
				engine_->draw(s, j * 64 + (int)(offset.x), i * 64 + (int)(offset.y), 0);
			}
		}
	}
}

void Map::load(int map_id){
	char filename[] = "mapXX.txt";
	char word[20];
	int i;
	//for portals
	int x, y, w, h;
	Portal p;
	FILE *in;
	
	//create file name string "mapXX.txt"
	filename[3] = (map_id % 100) / 10 + '0';
	filename[4] = map_id % 10 + '0';
	
	//open the file
	in = fopen(filename, "rt");
	
	if(!in){
		printf("could not open %s\n", filename);
		return;
	}
	
	//load values
	//read width/height on the first line
	fscanf(in, "%d%d", &width_, &height_);
	
	//remove the previous map content
	if(tiles_){
		delete[] tiles_;
	}
	tiles_ = new int[width_ * height_];
	
	portals_.clear();
	
	//read all the map values... yes, I advise against using fscanf but it's
	//easier to code...
	for(i = 0; i < width_ * height_; ++i){
		fscanf(in, "%d", tiles_ + i);
	}
	
	//read the data about the portals
	while(fscanf(in, "%s", word) == 1){
		if(strcmp(word, "portal") == 0){
			fscanf(in, "%d%d%d%d", &x, &y, &w, &h);
			p.space.pos.x = x * kTileSize;
			p.space.pos.y = y * kTileSize;
			p.space.dim.x = w * kTileSize;
			p.space.dim.y = h * kTileSize;
			fscanf(in, "%d%d%d", &x, &y, &w);
			p.dest_pos.x = x * kTileSize;
			p.dest_pos.y = y * kTileSize;
			p.dest_map = w;
			portals_.push_back(p);
		}
	}
	
	fclose(in);
}

void Map::set_player(Player *p){
	player_ = p;
}

void Map::set_map_transition(MapTransition *mt){
	map_transition_ = mt;
}

int Map::get_width(){
	return width_;
}

int Map::get_height(){
	return height_;
}

const int *Map::get_tiles(){
	return tiles_;
}
	