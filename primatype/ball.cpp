//Lithio
//December 27, 2020

#include <cstdio>

#include "ball.hpp"
#include "pwengine.hpp"

Ball::Ball(PWEngine *engine, int size, int layer_number, int color):
PWObject(std::string("ball"), engine, true),
vx_(100),
vy_(100),
x_(400),
y_(300),
layer_number_(layer_number),
sprite_(nullptr)
{
	sprite_ = engine_->load_image("test-images/ball.png");
}

Ball::~Ball(){
	SDL_DestroyTexture(sprite_);
}
	
//move the ball
void Ball::update(){
	//move the ball
	x_ += vx_ * engine_->deltatime() / 1000;
	y_ += vy_ * engine_->deltatime() / 1000;
	
	//when the ball is 200 pixels away from center
	if((x_-400)*(x_-400) + (y_-300)*(y_-300) > 200 * 200){
		vx_ = -vx_;
		vy_ = -vy_;
		//destroy();
		
		PWObject *new_ball = new Ball(engine_, 0, 5, 0);
	}
	
	//if the ctrl button is pressed, ball destroys
	if(engine_->is_key_pressed("Left Ctrl")
		|| engine_->is_key_pressed("Right Ctrl")){
		
		destroy();
	}
	
	//draw the ball
	SDL_Rect dst_rect;
	dst_rect.x = x_;
	dst_rect.y = y_;
	
	engine_->draw(sprite_, NULL, &dst_rect, layer_number_);
}
