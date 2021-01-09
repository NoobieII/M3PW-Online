//Lithio
//December 30, 2020

#ifndef SPACE_HPP
#define SPACE_HPP

class Space{
public:
	Space();
	Space(const Space &s);
	Space(float x_position, float y_position, float width = 0, float height = 0);
	
	//destructor
	~Space();
	
	//vector dot operator
	float operator*(const Space &s) const;
	
	float get_square_distance(const Space &s) const;
	
	//arithmetic operators, width and height
	Space operator-(const Space &s) const;
	Space operator+(const Space &s) const;
	
	//unary operators
	Space operator-() const;
	
	//collision with another space
	bool is_colliding(const Space &s) const;
	
	//get midpoint of the object
	Space get_midpoint() const;
	
	//get direction from midpoint of this space to midpoint of s, [-180, 180]
	float get_direction_towards(const Space &s) const;
	
	float x;
	float y;
	float w;
	float h;
};

#endif
