//test animation object
//Lithio
//April 18, 2021

#include "pwengine.hpp"
#include "test_dragon.hpp"

TestDragon::TestDragon(std::string name, PWEngine *engine):
PWObject(name, engine, true),
x_(0.0),
y_(0.0),
sprite_(nullptr)
{
	sprite_ = engine_->load_sprite("test-images/dragon.png", 72, 108, 0, 0, 2, 20);
}

TestDragon::~TestDragon(){
	delete sprite_;
}
	
void TestDragon::update(){
	//read from input
	float deltax = engine_->get_arrow_direction().x;
	float deltay = engine_->get_arrow_direction().y;
	
	x_ += deltax;
	y_ += deltay;
	
	//draw
	engine_->draw(sprite_, (int) x_, (int) y_, 1);
}
