//Lithio
//December 18, 2020

#ifndef PWENGINE_HPP
#define PWENGINE_HPP

#include <map>
#include <string>
#include <vector>

#include <SDL.h>

#include "space.hpp"
#include "pwsprite.hpp"

class PWObject;

class PWEngine{
public:
	//default constructor
	PWEngine();
	
	//destructor
	~PWEngine();
	
	//call this function to initialize the window and screen layers
	//returns 0 upon success or -1 upon failure
	int initialize(
		int screen_width,
		int screen_height,
		int framerate,
		const char *caption
	);
	
	//get a unique object id (don't call this function)
	int get_object_id();
	
	//add an object to the engine - call object.destroy() to free memory
	void add_object(PWObject *object);
	
	//search for existing object with the name - returns NULL if none found
	PWObject *get_object(const std::string &name);
	PWObject *get_object_by_id(int id);
	
	//collect everything from the event queue
	void poll_all_events();
	
	//calls update() for all active objects - destroys objects set to be
	//destroyed
	void update_all();
	
	//time difference in milliseconds since last update
	int deltatime();
	
	//number of game ticks since the start of the program
	int ticks();
	
	//draw to specified layer number - lower numbers take higher precedence.
	//layer number should be between 0-9
	void draw(SDL_Texture *src, SDL_Rect *src_rect, SDL_Rect *dst_rect, int layer_num);
	
	//same draw function but with simplified interface
	void draw(PWSprite *sprite, int x, int y, int layer_num);
	
	//draw text
	int draw_str_wrapped(const char *src, int x, int y, int w, int h, int len, int layer_num);
	//nitty gritty functions for getting position of cursor in text...
	
	
	//draw to current window
	void render();
	
	//sleep until start of next frame
	void sleep_until_next_frame();
	
	//load a SDL_Texture from an image file
	SDL_Texture *load_image(std::string path);
	
	//Getting a sprite:
	//const char *path:      path of sprite file
	//int w:                 dimensions of sprite
	//int h:
	//int x:                 position of sprite in image
	//int y:
	//int num_frame:         number of frames (moves down the image)
	//Returns a newly allocated sprite struct, or nullptr if not.
	//Use delete operator on sprite to deallocate.
	PWSprite *load_sprite(
		std::string path,
		int w,
		int h,
		int x,
		int y,
		int num_frames,
		int period
	);
	
	//mouse position
	int get_mouse_x();
	int get_mouse_y();
	
	//input events
	bool is_left_button_pressed();
	bool is_right_button_pressed();
	bool is_middle_button_pressed();
	
	bool is_left_button_lifted();
	bool is_right_button_lifted();
	bool is_middle_button_lifted();
	
	bool is_key_pressed(std::string key);
	bool is_key_lifted(std::string key);
	
	//returns a 2 dimensional vector <x, y> where x, y are elements of
	//[-1, 1] depending on the direction of the keys pressed
	Space get_arrow_direction();
	
	//text input - returns empty string if there is none
	const char *get_text_input();
	
	//returns true if the window X button was pressed
	bool is_quit();
private:
	
	int deltatime_;
	int framerate_;
	int ticks_;
	int last_update_time_;
	int last_sleep_time_;
	int next_sleep_time_;
	int object_id_;
	std::map<int, PWObject*> objects_;
	std::map<std::string, int> object_name_id_;
	
	SDL_Window *window_;
	SDL_Surface *window_surface_;
	SDL_Renderer *renderer_;
	SDL_Texture *charset_; //text sprite sheet
	std::vector<SDL_Texture*> layer_texture_[10];
	std::vector<SDL_Rect> layer_src_rect_[10];
	std::vector<SDL_Rect> layer_dst_rect_[10];
	
	std::map<std::string, SDL_Texture*> loaded_textures_;
	
	//mouse position state
	int mouse_x_;
	int mouse_y_;
	
	//mouse button event
	bool is_left_button_pressed_;
	bool is_right_button_pressed_;
	bool is_middle_button_pressed_;
	
	bool is_left_button_lifted_;
	bool is_right_button_lifted_;
	bool is_middle_button_lifted_;
	
	//mouse scrolling motion event
	int scroll_x_;
	int scroll_y_;
	
	//keyboard events
	std::map<std::string, bool> is_key_pressed_;
	std::map<std::string, bool> is_key_lifted_;
	
	//direction from arrow keys
	int direction_;
	
	//text input from keyboard
	char text_input_[32];
	
	//window X button
	bool is_quit_;
};

#endif
