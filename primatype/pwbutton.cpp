//Lithio
//June 2, 2021

//implementation of a button class
#include "pwbutton.hpp"
#include "pwengine.hpp"

//for printf debugging
#include <cstdio>

PWButton::PWButton(
	int x,
	int y,
	int width,
	int height,
	int sprite_layer,
	PWSprite *sprite_unpushed,
	PWSprite *sprite_pushed,
	std::string name,
	PWEngine *engine
):
PWObject(name, engine, true),
x_(x),
y_(y),
w_(width),
h_(height),
state_hover_(0),
state_push_(0),
sprite_layer_(sprite_layer),
sprite_unpushed_(sprite_unpushed),
sprite_pushed_(sprite_pushed)
{
}

//destructor
PWButton::~PWButton(){
}

//this function is called once during each game tick
void PWButton::update(){
	int mouse_x;
	int mouse_y;
	bool is_mouse_hovered_over;
	
	mouse_x = engine_->get_mouse_x();
	mouse_y = engine_->get_mouse_y();
	
	is_mouse_hovered_over = mouse_x >= x_ && mouse_x < x_ + w_
		&& mouse_y >= y_ && mouse_y < y_ + h_;
	
	//hover state machine
	switch(state_hover_){
	case 0:
		//if left mouse button is pressed and mouse is hovered over button
		if(is_mouse_hovered_over){
			state_hover_ = 1;
		}
		break;
	//button hovered over state
	case 1:
		state_hover_ = 2;
		break;
	case 2:
		//if the mouse is hovering over, stay in the state
		//else return to original state
		if(!is_mouse_hovered_over){
			state_hover_ = 0;
		}
		break;
	}
	
	//push state machine
	switch(state_push_){
	case 0:
		//if left mouse button is pressed and mouse is hovered over button
		if(engine_->is_left_button_pressed() && is_mouse_hovered_over){
			
			//if the button is pressed and lifted very quickly (same frame)
			if(engine_->is_left_button_lifted()){
				state_push_ = 4;
			}
			else{
				state_push_ = 1;
			}
		}
		break;
	//button pushed state
	case 1:
		//if left mouse button is released and mouse is not hovered over button
		if(!engine_->is_left_button_lifted()){
			state_push_ = 2;
		}
		else if(is_mouse_hovered_over){
			state_push_ = 3;
		}
		else{
			state_push_ = 0;
		}
		break;
	//waiting for left mouse button to be released
	case 2:
		if(engine_->is_left_button_lifted()){
			if(is_mouse_hovered_over){
				state_push_ = 3;
			}
			else{
				state_push_ = 0;
			}
		}
		break;
	//button released state
	case 3:
		state_push_ = 0;
		break;
	//button pushed and released at same time
	case 4:
		state_push_ = 0;
		break;
	}
	
	//render
	if((state_push_ == 1 || state_push_ == 2) && is_mouse_hovered_over){
		engine_->draw(sprite_pushed_, x_, y_, sprite_layer_);
	}
	else{
		engine_->draw(sprite_unpushed_, x_, y_, sprite_layer_);
	}
}

//these functions return true during only one game tick 

//returns true when the mouse cursor is hovered over the button
bool PWButton::is_hover_triggered(){
	return state_hover_ == 1;
}

//returns true when the left mouse button is pressed while the mouse
//cursor is over the button
bool PWButton::is_push_triggered(){
	return state_push_ == 1 || state_push_ == 4;
}

//returns true when the left mouse button is released while the mouse
//cursor is over the button, only after the button has been pushed.
//This should be checked if the button performs any function.
bool PWButton::is_release_triggered(){
	return state_push_ == 3 || state_push_ == 4;
}
