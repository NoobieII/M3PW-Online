//Lithio
//June 10, 2021

#ifndef PWTEXTBOX_HPP
#define PWTEXTBOX_HPP

#include "pwobject.hpp"
#include "pwsprite.hpp"

//text box interface
class PWTextbox: public PWObject{
public:
	//constructor
	PWTextbox(
		int x,
		int y,
		int w,
		int h,
		int size,
		int sprite_layer,
		bool accept_newlines,
		bool censored,
		PWSprite *underlay,
		std::string name,
		PWEngine *engine
	);
	
	//destructor
	~PWTextbox();
	
	//called once every game tick
	void update();
	
	//returns the string of the textbox
	const char *get_str();
	
	//clears all the text
	void clear();
	
	//send true for the textbox to read input, send false for the textbox
	//to not read input (same effect as clicking on the textbox or clicking
	//outside)
	void set_active(bool state);
	
private:
	int x_;                  //position on screen
	int y_;
	int w_;                  //dimensions
	int h_;
	int size_;               //max length of string
	int sprite_layer_;       //layer to render
	bool accept_newlines_;   //pressing enter
	bool censored_;
	PWSprite *underlay_;
	
	int pos_;
	int len_;
	char *str_;
	char *str_censored_;
	int state_;
	PWSprite *cursor_sprite_;
	
	//for optimization purposes, save cursor position so no need to recalculate
	bool recalculate_cursor_position_;
	int cursor_x_;
	int cursor_y_;
};

#endif
