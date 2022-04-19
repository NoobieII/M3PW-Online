//Lithio
//March 7, 2022

#include "constants.hpp"
#include "map.hpp"
#include "map_transition.hpp"
#include "player.hpp"

MapTransition::MapTransition(PWEngine *engine, Player *p, Map *m):
PWObject("map_transition", engine, true),
state_(0),
player_(p),
map_(m),
dest_pos_(0.0f, 0.0f),
dest_map_(0)
{
	int i;
	
	for(i = 0; i < 8; ++i){
		sprites_[i] = engine_->load_sprite("sprites/map_transition.png", 8, 8, i * 8, 0, 1, 1);
	}
}

MapTransition::~MapTransition(){
	int i;
	
	for(i = 0; i < 8; ++i){
		delete sprites_[i];
	}
}

void MapTransition::update(){
	int i, j;
	int frame;
	
	if(state_ == 0){
		return;
	}
	else if(state_ > 0 && state_ < 9){
		//draw the map transition
		frame = state_ - 1;
	}
	else if(state_ == 9){
		map_->deactivate();
		map_->load(dest_map_);
		frame = 7;
	}
	else if(state_ > 9 && state_ < 9 + kFrameRate){
		frame = 7;
	}
	else if(state_ >= 9 + kFrameRate && state_ < 9 + kFrameRate + 8){
		if(state_ == 9 + kFrameRate){
			map_->activate();
			player_->set_position(dest_pos_);
		}
		frame = 7 - (state_ - 9 - kFrameRate);
	}
	else{
		player_->activate();
		
		state_ = 0;
		return;
	}
	
	//draw the map transition effect
	if(state_ != 0){
		for(i = 0; i < kScreenHeight / 8; ++i){
			for(j = 0; j < kScreenWidth / 8; ++j){
				engine_->draw(sprites_[frame], j * 8, i * 8, 0);
			}
		}
	}
	
	state_++;
}

void MapTransition::move_player_to_map(Portal *p){
	if(state_ == 0){
		state_ = 1;
	}
	
	dest_pos_ = p->dest_pos;
	dest_map_ = p->dest_map;
	
	player_->deactivate();
	//map_->deactivate();
}

