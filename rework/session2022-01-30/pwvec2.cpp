//Lithio
//February 25, 2022

#include "pwvec2.hpp"

PWVec2::PWVec2(float x_position, float y_position):
x(x_position),
y(y_position)
{
}

//vector dot operator
float PWVec2::operator*(const PWVec2 &s) const{
	return x * s.x + y * s.y;
}

float PWVec2::square_dist() const{
	return x * x + y * y;
}

//arithmetic operators
PWVec2 PWVec2::operator-(const PWVec2 &s) const{
	return PWVec2(x - s.x, y - s.y);
}

PWVec2 PWVec2::operator+(const PWVec2 &s) const{
	return PWVec2(x + s.x, y + s.y);
}

//unary operators
PWVec2 PWVec2::operator-() const{
	return PWVec2(-x, -y);
}

PWVec2::~PWVec2(){
}

PWVec2 operator*(float k, const PWVec2 &s){
	return PWVec2(k * s.x, k * s.y);
}

PWVec2 operator*(const PWVec2 &s, float k){
	return k * s;
}
