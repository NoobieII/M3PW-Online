//Lithio
//February 25, 2022

#ifndef PWVEC2_HPP
#define PWVEC2_HPP

typedef struct PWVec2{
	PWVec2() = default;
	PWVec2(const PWVec2 &v) = default;
	PWVec2(float x_position, float y_position);
	
	//vector dot operator
	float operator*(const PWVec2 &s) const;
	
	float square_dist() const;
	
	//arithmetic operators
	PWVec2 operator-(const PWVec2 &s) const;
	PWVec2 operator+(const PWVec2 &s) const;
	
	//unary operators
	PWVec2 operator-() const;
	
	~PWVec2();
	
	float x;
	float y;
} PWVec2;

PWVec2 operator*(float k, const PWVec2 &s);
PWVec2 operator*(const PWVec2 &s, float k);

#endif