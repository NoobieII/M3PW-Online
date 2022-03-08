//Lithio
//February 21, 2022

#include "camera.hpp"
#include "constants.hpp"
#include "map.hpp"
#include "player.hpp"

Player::Player(PWEngine *engine):
PWObject("player", engine, true),
space_(0.0f, 0.0f, 48.0f, 48.0f),
map_(nullptr),
sprite_(nullptr),
sprite_up_(nullptr),
sprite_down_(nullptr),
sprite_left_(nullptr),
sprite_right_(nullptr)
{
	sprite_up_ = engine->load_sprite("sprites.png", 64, 64, 0, 0, 1, 1);
	sprite_down_ = engine->load_sprite("sprites.png", 64, 64, 64, 0, 1, 1);
	sprite_left_ = engine->load_sprite("sprites.png", 64, 64, 128, 0, 1, 1);
	sprite_right_ = engine->load_sprite("sprites.png", 64, 64, 192, 0, 1, 1);
	sprite_ = sprite_right_;
}

Player::~Player(){
	delete sprite_up_;
	delete sprite_down_;
	delete sprite_left_;
	delete sprite_right_;
}
	
//to be called every game tick
void Player::update(){
	PWVec2 movement;
	PWVec2 offset;
	
	//walking
	space_.set_relaxation_time(0.1f);
	movement = 192.0f * engine_->get_arrow_direction();
	space_.approach_velocity(movement);
	space_.update_position(engine_->deltatime()/1000.0f);
	
	//if colliding with map
	if(space_.is_colliding(map_->get_tiles(), map_->get_width(), map_->get_height(), kTileSize)){
		space_.stop_and_undo();
	}
	
	//TODO: draw on map with camera position
	if(movement.square_dist() > 0.0f){
		if(movement.y * movement.y < movement.x * movement.x){
			//left or right side
			if(movement.x > 0.0f){
				sprite_ = sprite_right_;
			}
			else{
				sprite_ = sprite_left_;
			}
		}
		else{
			//up or down
			if(movement.y > 0.0f){
				sprite_ = sprite_down_;
			}
			else{
				sprite_ = sprite_up_;
			}
		}
	}
	
	//draw the player on layer 1
	offset = camera::offset(this);
	//printf("offset = %f, %f position = %f, %f   ", offset.x, offset.y, space_.pos.x, space_.pos.y);
	//printf("movement = %f, %f\n", movement.x, movement.y);
	//printf("midpoint = %f, %f\n", space_.midpoint().x, space_.midpoint().y);
	engine_->draw(sprite_, space_.pos.x + offset.x - 8, space_.pos.y + offset.y - 8, 1);
}
	
const Space &Player::get_space(){
	return space_;
}

void Player::set_position(int x, int y){
	space_.stop_and_undo();
	space_.reset_position(PWVec2((float)x, (float)y));
}

void Player::set_position(PWVec2 pos){
	space_.reset_position(pos);
}

void Player::set_map(Map *m){
	map_ = m;
}

Map *Player::get_map(){
	return map_;
}
