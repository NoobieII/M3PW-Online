//Lithio
//December 30, 2020
//upgraded February 18, 2022 for motion

#ifndef SPACE_HPP
#define SPACE_HPP

#include "pwvec2.hpp"

class Space{
public:
	Space() = default;
	Space(const Space &s) = default;
	Space(float x_position, float y_position, float width = 0, float height = 0);
	
	//destructor
	~Space();
	
	//collision with another space
	bool is_colliding(const Space &s) const;
	
	//collision with the map
	//tiles is the tile array from map file
	//width and height are the dimensions of the 2d array
	//tilesize is the side length of each tile (assumed to be square)
	bool is_colliding(const int *tiles, int width, int height, int tilesize);
	
	//get midpoint of the object
	PWVec2 midpoint() const;
	
	//get direction from midpoint of this space to midpoint of s, [-180, 180]
	float get_direction_towards(const Space &s) const;
	
	//motion
	
	//sets relaxation time (1/k). High t takes longer to change speed
	//Ex. 0.5 for walking and 5 for skating on ice
	void set_relaxation_time(float t);
	
	//this sets the acceleration to reach the given velocity
	void approach_velocity(float vel_x, float vel_y);
	void approach_velocity(const PWVec2 &s);
	
	//compute acceleration, velocity and position
	void update_position(float deltatime);
	
	//set velocity and acceleration to 0 and undo the last position change
	//usually if a collision is detected
	void stop_and_undo();
	
	//position, direction and dimensions
	PWVec2 pos;
	PWVec2 dir;
	PWVec2 dim;
	
private:
	//previous position, acceleration, velocity, velocity decay constant
	PWVec2 prev_pos_;
	PWVec2 vel_;
	PWVec2 acc_;
	//decay constant
	float k_;
};

Space operator*(float k, const Space &s);
Space operator*(const Space &s, float k);

#endif
