//Lithio
//December 30, 2020

#include <cmath> //for atan2

#include "space.hpp"

Space::Space():
x(0),
y(0),
w(0),
h(0)
{
}

Space::Space(const Space &s):
x(s.x),
y(s.y),
w(s.w),
h(s.h)
{
}

Space::Space(float x_position, float y_position, float width, float height):
x(x_position),
y(y_position),
w(width),
h(height)
{
}

Space::~Space(){
}

//vector dot operator
float Space::operator*(const Space &s) const{
	return x*s.x + y*s.y;
}

float Space::get_square_distance(const Space &s) const{
	return (x-s.x)*(x-s.x) + (y-s.y)*(y-s.y);
}

//arithmetic operators
Space Space::operator-(const Space &s) const{
	return Space(x - s.x, y - s.y, 0.0, 0.0);
}

Space Space::operator+(const Space &s) const{
	return Space(x + s.x, y + s.y, 0.0, 0.0);
}

//unary operators
Space Space::operator-() const{
	return Space(-x, -y, w, h);
}

//collision with another space
bool Space::is_colliding(const Space &s) const{
	bool collision_detected = false;
	
	//check if the horizontal range and vertical range are met
	if(s.x - w < x && x < s.x + s.w && s.y - h < y && y < s.y + s.h){
		collision_detected = true;
	}
	
	return collision_detected;
}

//get relative direction in degrees clockwise, starting at 0 at the right
float Space::get_direction_towards(const Space &s) const{
	float diff_x;
	float diff_y;
	double angle;
	
	Space midpoint = get_midpoint();
	Space dest_midpoint = s.get_midpoint();
	
	diff_x = dest_midpoint.x - midpoint.x;
	diff_y = midpoint.y - dest_midpoint.y;
	
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

Space Space::get_midpoint() const{
	return Space(x + w/2.0, y + h/2.0, 0.0, 0.0);
}

Space operator*(float k, const Space &s){
	return Space(k * s.x, k * s.y, s.w, s.h);
}
