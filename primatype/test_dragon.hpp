//example animated object
//Lithio
//April 18, 2021

#ifndef TESTDRAGON_HPP
#define TESTDRAGON_HPP

#include "pwobject.hpp"
#include "pwsprite.hpp"

class TestDragon: public PWObject{
public:
	TestDragon(std::string name, PWEngine *engine);
	~TestDragon();
	
	void update();
private:
	float x_;
	float y_;
	PWSprite *sprite_;
};

#endif