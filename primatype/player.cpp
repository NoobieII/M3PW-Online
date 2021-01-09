//Lithio
//January 8, 2021

#include "constants.hpp"
#include "player.hpp"

#include <cstdio>

Player::Player(PWEngine *engine):
PWObject("player", engine, true),
position_(400, 300, 32, 96),
direction_(0),
velocity_x_(0.0),
velocity_y_(0.0),
is_flying_(false),
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
	
	//determine direction
	if(engine_->is_key_pressed("Right")){
		direction_ |= 0x1;
	}
	if(engine_->is_key_pressed("Up")){
		direction_ |= 0x2;
	}
	if(engine_->is_key_pressed("Left")){
		direction_ |= 0x4;
	}
	if(engine_->is_key_pressed("Down")){
		direction_ |= 0x8;
	}
	if(engine_->is_key_lifted("Right")){
		direction_ &= 0xe;
	}
	if(engine_->is_key_lifted("Up")){
		direction_ &= 0xd;
	}
	if(engine_->is_key_lifted("Left")){
		direction_ &= 0xb;
	}
	if(engine_->is_key_lifted("Down")){
		direction_ &= 0x7;
	}
	
	switch(direction_){
	case 0x1: //right
		velocity_increment_x = increment_speed;
		velocity_increment_y = 0.0;
		break;
	case 0x3: //top right
		velocity_increment_x = 0.71 * increment_speed;
		velocity_increment_y = -0.71 * increment_speed;
		break;
	case 0x2: //up
		velocity_increment_x = 0.0;
		velocity_increment_y = -increment_speed;
		break;
	case 0x6: //top left
		velocity_increment_x = -0.71 * increment_speed;
		velocity_increment_y = -0.71 * increment_speed;
		break;
	case 0x4: //left
		velocity_increment_x = -increment_speed;
		velocity_increment_y = 0.0;
		break;
	case 0xc: //bottom left
		velocity_increment_x = -0.71 * increment_speed;
		velocity_increment_y = 0.71 * increment_speed;
		break;
	case 0x8: //down
		velocity_increment_x = 0.0;
		velocity_increment_y = increment_speed;
		break;
	case 0x9: //bottom right
		velocity_increment_x = 0.71 * increment_speed;
		velocity_increment_y = 0.71 * increment_speed;
		break;
	default:
		velocity_increment_x = 0.0;
		velocity_increment_y = 0.0;
		break;
	}
	
	//update velocity
	velocity_x_ = (velocity_x_ + velocity_increment_x)*(1.0 - drag);
	velocity_y_ = (velocity_y_ + velocity_increment_y)*(1.0 - drag);
	
	//update position
	position_.x += velocity_x_;
	position_.y += velocity_y_;
	
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
