//Lithio
//January 16, 2021

#include "constants.hpp"
#include "player.hpp"

#include <cstdio>

Player::Player(PWEngine *engine):
PWObject("player", engine, true),
position_(400, 300, 32, 96),
last_position_on_ground_(),
direction_(0),
velocity_x_(0.0),
velocity_y_(0.0),
is_flying_(false),
hp_(0),
max_hp_(0),
mp_(0),
max_mp_(0),
exp_(0),
max_exp_(0),
fly_(0),
max_fly_(0),
name_(""),
monster_kills_(),
npc_variables_(),
body_texture_(NULL),
wing_texture_(NULL),
shadow_texture_(NULL)
{
	body_texture_ = engine_->load_image("sprites/player.png");
	wing_texture_ = engine_->load_image("sprites/player-wings.png");
	shadow_texture_ = engine_->load_image("sprites/shadow.png");
}

Player::~Player(){
}

void Player::update(){
	float velocity_increment_x;
	float velocity_increment_y;
	
	float increment_speed;
	float drag;
	int movement_direction;
	
	//check keyboard to see which direction to move or to start flying
	if(is_flying_ == false){
		if(engine_->is_key_pressed("Space")){
			is_flying_ = true;
		}
	}
	else{
		if(engine_->is_key_lifted("Space")){
			is_flying_ = false;
		}
	}
	
	if(is_flying_){
		increment_speed = kPlayerSpeedIncrementFlying;
		drag = kPlayerDragFlying;
	}
	else{
		increment_speed = kPlayerSpeedIncrementWalking;
		drag = kPlayerDragWalking;
	}
	
	Space velocity_increment = increment_speed * engine_->get_arrow_direction();
	
	//update velocity
	velocity_x_ = (velocity_x_ + velocity_increment.x)*(1.0 - drag);
	velocity_y_ = (velocity_y_ + velocity_increment.y)*(1.0 - drag);
	
	//update position
	position_.x += velocity_x_;
	position_.y += velocity_y_;
	
	//TODO: collision detection
	
	
	
	//draw player to layer 1
	//draw wings behind player
	SDL_Rect dst_rect;
	
	if(is_flying_){
		dst_rect.x = position_.x - 32;
		dst_rect.y = position_.y;
		engine_->draw(wing_texture_, NULL, &dst_rect, 1);
	}
	
	dst_rect.x = position_.x - 16;
	dst_rect.y = position_.y;
	engine_->draw(body_texture_, NULL, &dst_rect, 1);
	
	//draw shadow to layer 2
	dst_rect.x = position_.x;
	dst_rect.y = position_.y + 64;
	engine_->draw(shadow_texture_, NULL, &dst_rect, 1);
}
