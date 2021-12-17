//Lithio
//June 2, 2021

//Button class

#ifndef PWBUTTON_HPP
#define PWBUTTON_HPP

#include "pwobject.hpp"
#include "pwsprite.hpp"

class PWButton: public PWObject{
public:
	//default constructor
	PWButton(
		int x,
		int y,
		int width,
		int height,
		int sprite_layer,
		PWSprite *sprite_unpushed,
		PWSprite *sprite_pushed,
		std::string name,
		PWEngine *engine
	);
	
	//destructor
	~PWButton();
	
	//this function is called once during each game tick
	void update();
	
	//these functions return true during only one game tick 
	
	//returns true when the mouse cursor is hovered over the button
	bool is_hover_triggered();
	
	//returns true when the left mouse button is pressed while the mouse
	//cursor is over the button
	bool is_push_triggered();
	
	//returns true when the left mouse button is released while the mouse
	//cursor is over the button, only after the button has been pushed.
	//This should be checked if the button performs any function.
	bool is_release_triggered();
private:
	int x_;
	int y_;
	int w_;
	int h_;
	int state_hover_;
	int state_push_;
	int sprite_layer_;
	PWSprite *sprite_unpushed_;
	PWSprite *sprite_pushed_;
};

#endif
