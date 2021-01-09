//Lithio
//December 30, 2020

#include <cstdio>

#include "space.hpp"

void print_space(const Space &s);

int main(){
	Space s1(4, 5);
	Space s2(6, 7);
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("s1 + s2 = ");
	print_space(s1+s2);
	printf("s1 - s2 = ");
	print_space(s1-s2);
	printf("-(s1 + s2) = ");
	print_space(-(s1 + s2));
	
	printf("midpoint of s1: ");
	print_space(s1.get_midpoint());
	printf("midpoint of s2: ");
	print_space(s2.get_midpoint());
	
	printf("dot product: %d\n", s1*s2);
	
	//test collision of 2 spaces
	s1.w = 50;
	s1.h = 50;
	s2.w = 50;
	s2.h = 50;
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = 50;
	s2.y = 50;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 0)\n\n", s1.is_colliding(s2));
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = 49;
	s2.y = 50;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 0)\n\n", s1.is_colliding(s2));
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = 50;
	s2.y = 49;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 0)\n\n", s1.is_colliding(s2));
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = 49;
	s2.y = 49;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 1)\n\n", s1.is_colliding(s2));
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = -50;
	s2.y = -50;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 0)\n\n", s1.is_colliding(s2));
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = -49;
	s2.y = -50;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 0)\n\n", s1.is_colliding(s2));
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = -50;
	s2.y = -49;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 0)\n\n", s1.is_colliding(s2));
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = -49;
	s2.y = -49;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 1)\n\n", s1.is_colliding(s2));
	
	//third corner
	s1.x = 0;
	s1.y = 0;
	
	s2.x = 50;
	s2.y = -50;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 0)\n\n", s1.is_colliding(s2));
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = 49;
	s2.y = -50;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 0)\n\n", s1.is_colliding(s2));
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = 50;
	s2.y = -49;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 0)\n\n", s1.is_colliding(s2));
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = 49;
	s2.y = -49;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 1)\n\n", s1.is_colliding(s2));
	
	//a more arbitrary space
	s1.w = 30;
	s1.h = 50;
	s2.w = 40;
	s2.h = 70;
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = 30;
	s2.y = 50;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 0)\n\n", s1.is_colliding(s2));
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = 29;
	s2.y = 50;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 0)\n\n", s1.is_colliding(s2));
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = 30;
	s2.y = 49;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 0)\n\n", s1.is_colliding(s2));
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = 29;
	s2.y = 49;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 1)\n\n", s1.is_colliding(s2));
	
	//top left corner
	s1.x = 0;
	s1.y = 0;
	
	s2.x = -40;
	s2.y = -70;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 0)\n\n", s1.is_colliding(s2));
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = -39;
	s2.y = -70;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 0)\n\n", s1.is_colliding(s2));
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = -40;
	s2.y = -69;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 0)\n\n", s1.is_colliding(s2));
	
	s1.x = 0;
	s1.y = 0;
	
	s2.x = -39;
	s2.y = -69;
	
	printf("s1 = ");
	print_space(s1);
	printf("s2 = ");
	print_space(s2);
	
	printf("angle from s1 to s2: %d\n", s1.get_direction_towards(s2));
	printf("collision: %d (expected 1)\n\n", s1.is_colliding(s2));
	
	return 0;
}

void print_space(const Space &s){
	printf("(%d, %d) \n", s.x, s.y);
}
