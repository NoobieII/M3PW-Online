//Lithio
//December 30, 2020

#include <cmath> //for atan2

#include "space.hpp"

Space::Space(float x_position, float y_position, float width, float height):
pos(x_position, y_position),
dir(1.0f, 0.0f),
dim(width, height),
prev_pos_(x_position, y_position),
vel_(0.0f, 0.0f),
acc_(0.0f, 0.0f),
k_(0.0f)
{
}

Space::~Space(){
}

//collision with another space
bool Space::is_colliding(const Space &s) const{
	bool collision_detected = false;
	
	//check if the horizontal range and vertical range are met
	if(s.pos.x - dim.x < pos.x && pos.x < s.pos.x + s.dim.x && s.pos.y - dim.y < pos.y && pos.y < s.pos.y + s.dim.y){
		collision_detected = true;
	}
	
	return collision_detected;
}

bool Space::is_colliding(const int *tiles, int width, int height, int tilesize){
	int lhs = (int)(pos.x) / tilesize;
	int rhs = ((int)(pos.x + dim.x) + tilesize - 1) / tilesize;
	int top = (int)(pos.y) / tilesize;
	int bot = ((int)(pos.y + dim.y) + tilesize - 1) / tilesize;
	int i;
	int j;
	
	//if any part of the space is outside of the map
	if((int)(pos.x) < 0 || (int)(pos.x + dim.x) > tilesize * width || (int)(pos.y) < 0 || (int)(pos.y + dim.y) > tilesize * height){
		return true;
	}
	
	//if any part of space is colliding with the map spaces
	for(i = top; i < bot; ++i){
		for(j = lhs; j < rhs; ++j){
			if(tiles[j + i * width] % 2){
				return true;
			}
		}
	}
	
	return false;
}

//get relative direction in degrees clockwise, starting at 0 at the right
float Space::get_direction_towards(const Space &s) const{
	float diff_x;
	float diff_y;
	double angle;
	
	
	diff_x = s.midpoint().x - midpoint().x;
	diff_y = midpoint().y - s.midpoint().y;
	
	if(diff_x == 0.0 && diff_y == 0.0){
		return 90.0;
	}
	
	angle = atan2(diff_y, diff_x) * 180.0 / 3.14159265;
	
	//round to nearest integer
	if(angle < 0.0){
		angle -= 0.5;
	}
	else{
		angle += 0.5;
	}
	
	return (float)(angle);
}

PWVec2 Space::midpoint() const{
	return PWVec2(pos.x + dim.x/2.0f, pos.y + dim.y/2.0f);
}

void Space::set_relaxation_time(float t){
	k_ = 1.0f / t;
}

void Space::approach_velocity(float vel_x, float vel_y){
	acc_ = k_ * (PWVec2(vel_x, vel_y) - vel_);
}

void Space::approach_velocity(const PWVec2 &s){
	approach_velocity(s.x, s.y);
}

void Space::update_position(float deltatime){
	/*
	float new_x  = x + deltatime * ( vx *  0.5 * ax * deltatime);
	float new_ax = ax - k * vx;
	float new_vx = vx + (ax + new_ax) * (deltatime * 0.5);
	
	float new_y  = y + deltatime * ( vy *  0.5 * ay * deltatime);
	float new_ay = ay - k * vy;
	float new_vy = vy + (ay + new_ay) * (deltatime * 0.5);
	*/
	
	PWVec2 new_pos = pos + deltatime * (vel_ + 0.5f * acc_ * deltatime);
	PWVec2 new_acc = acc_;// - k_ * vel_;
	PWVec2 new_vel = vel_ + (acc_ + new_acc) * (deltatime * 0.5f);
	/*
	prev_x = x;
	prev_y = y;
	
	x  = new_x;
	vx = new_vx;
	ax = new_ax;
	
	y  = new_y;
	vy = new_vy;
	ay = new_ay;
	*/
	
	prev_pos_ = pos;
	pos = new_pos;
	vel_ = new_vel;
	acc_ = new_acc;
}

void Space::stop_and_undo(){
	/*
	x  = prev_x;
	vx = 0;
	ax = 0;
	y  = prev_y;
	vy = 0;
	ay = 0;
	*/
	pos = prev_pos_;
	vel_ = PWVec2(0.0f, 0.0f);
	acc_ = PWVec2(0.0f, 0.0f);
}

void Space::reset_position(PWVec2 position){
	pos = position;
	prev_pos_ = pos;
	vel_ = PWVec2(0.0f, 0.0f);
	acc_ = PWVec2(0.0f, 0.0f);
}
