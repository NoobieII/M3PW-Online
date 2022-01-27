//Primatype 2d game engine
//For debug, define PW_WATCH_CONTROLS
//
//
//Lithio
//December 18, 2020

//for clock()
#include <ctime>
#include <cstdio> //for printf
#include <cstring> //for strlen

//these includes are for timing the sleeping correctly
#if (defined(unix) || defined(__unix) || defined(__unix__))
#include "unistd.h"
#endif

#if defined(_WIN32)
#include "windows.h"
#endif

#include <SDL_image.h>

#include "pwengine.hpp"
#include "pwobject.hpp"

PWEngine::PWEngine():
deltatime_(0),
framerate_(0),
ticks_(0),
last_update_time_(0),
last_sleep_time_(0),
next_sleep_time_(0),
object_id_(0),
objects_(),
object_name_id_(),
window_(nullptr),
window_surface_(nullptr),
renderer_(nullptr),
charset_(nullptr),
layer_texture_(),
layer_src_rect_(),
layer_dst_rect_(),
loaded_textures_(),
mouse_x_(0),
mouse_y_(0),
is_left_button_pressed_(false),
is_right_button_pressed_(false),
is_middle_button_pressed_(false),
is_left_button_lifted_(false),
is_right_button_lifted_(false),
is_middle_button_lifted_(false),
scroll_x_(0),
scroll_y_(0),
is_key_pressed_(),
is_key_lifted_(),
direction_(0),
text_input_(),
is_quit_(false)
{
}

PWEngine::~PWEngine(){
	int i;
	std::map<int, PWObject*>::iterator it;
	std::map<std::string, SDL_Texture*>::iterator it2;
	
	//destroy renderer
	SDL_DestroyRenderer(renderer_);
	
	//destroy the window (and its surface) object
	SDL_DestroyWindow(window_);
	
	//destroy all objects in the game
	for(it = objects_.begin(); it != objects_.end(); ++it){
		delete it->second;
	}
	
	//destroy all sprites
	for(it2 = loaded_textures_.begin(); it2 != loaded_textures_.end(); ++it2){
		if(it2->second){
			SDL_DestroyTexture(it2->second);
		}
	}
	
	//destroy charset spritesheet
	if(charset_){
		SDL_DestroyTexture(charset_);
	}
	
	IMG_Quit();
	
	//close SDL
	SDL_Quit();
}

int PWEngine::initialize(int screen_width, int screen_height, int framerate, const char *caption){
	int i;
	
	//Initialize SDL2
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("Error in initialization. %s\n", SDL_GetError());
		return -1;
	}
	
	//Initialize SDL2 image
	if(IMG_Init(IMG_INIT_JPG & IMG_INIT_PNG) != (IMG_INIT_JPG & IMG_INIT_PNG)){
		printf("Error in initializing IMG: %s\n", IMG_GetError());
		return -1;
	}
	
	//make there be no interpolation!
	if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0") == SDL_FALSE){
		printf("Error setting scaling to nearest pixel sampling\n");
		return -1;
	}
	
	//initialize window, screen_width x screen_height, caption "Primatype"
	window_ = SDL_CreateWindow(caption, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
	
	if(!window_){
		printf("Error in initialization. %s\n", SDL_GetError());
		return -1;
	}
	
	//get reference to window's surface
	window_surface_ = SDL_GetWindowSurface(window_);
	
	//initialize renderer
	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
	if(!renderer_){
		printf("Could not create renderer. %s\n", SDL_GetError());
		return -1;
	}
	
	//the amount of rgb values to draw on the rendering target
	SDL_SetRenderDrawColor(renderer_, 0xff, 0xff, 0xff, 0xff);
	
	
	//initialize text
	charset_ = load_image("sprites/charset2.png");
	
	#if (defined(unix) || defined(__unix) || defined(__unix__))
	struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC, &tp);
	
	last_update_time_ = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
	#endif
	
	#if defined(_WIN32)
	last_update_time_ = clock() / (CLOCKS_PER_SEC / 1000);
	#endif
	
	//initialize time
	framerate_ = framerate;
	last_sleep_time_ = last_update_time_;
	next_sleep_time_ = last_update_time_ + (1000 / framerate_);
	
	return 0;
}

//get a unique object id (don't call this function)
int PWEngine::get_object_id(){
	return object_id_++;
}
	
//add an object to the engine - call object.destroy() to free memory
void PWEngine::add_object(PWObject *object){
	//add the string to id record
	object_name_id_[object->get_name()] = object->get_id();
	
	//add the id to object record
	objects_[object->get_id()] = object;
}

PWObject *PWEngine::get_object(const std::string &name){
	int object_id;
	
	//get an object id from the name
	std::map<std::string, int>::iterator it = object_name_id_.find(name);
	
	if(it == object_name_id_.end()){
		printf("PWEngine: could not find object with name %s.\n", name.c_str());
		return nullptr;
	}
	
	object_id = object_name_id_[name];
	
	return objects_[object_id];
}

PWObject *PWEngine::get_object_by_id(int id){
	std::map<int, PWObject*>::iterator it = objects_.find(id);
	if(it == objects_.end()){
		printf("PWEngine: could not find object with id %d.\n", id);
		return nullptr;
	}
	else{
		return objects_[id];
	}
}

#if not defined PW_WATCH_CONTROLS

//collect everything from the event queue
void PWEngine::poll_all_events(){
	SDL_Event event;
	SDL_Keycode keycode;
	
	//read all queued input events
	while(SDL_PollEvent(&event)){
		switch(event.type){
		case SDL_QUIT:
			is_quit_ = true;
			//printf("window X button pressed\n");
			break;
		case SDL_MOUSEMOTION:
			mouse_x_ = event.motion.x;
			mouse_y_ = event.motion.y;
			//printf("mouse at (%d, %d)\n", mouse_x_, mouse_y_);
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch(event.button.button){
			case SDL_BUTTON_LEFT:
				is_left_button_pressed_ = true;
				//printf("mouse left button pressed\n");
				break;
			case SDL_BUTTON_RIGHT:
				is_right_button_pressed_ = true;
				//printf("mouse right button pressed\n");
				break;
			case SDL_BUTTON_MIDDLE:
				is_middle_button_pressed_ = true;
				//printf("mouse middle button pressed\n");
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch(event.button.button){
			case SDL_BUTTON_LEFT:
				is_left_button_lifted_ = true;
				//printf("mouse left button lifted\n");
				break;
			case SDL_BUTTON_RIGHT:
				is_right_button_lifted_ = true;
				//printf("mouse right button lifted\n");
				break;
			case SDL_BUTTON_MIDDLE:
				is_middle_button_lifted_ = true;
				//printf("mouse middle button lifted\n");
				break;
			}
			break;
		case SDL_MOUSEWHEEL:
			scroll_x_ = event.wheel.x;
			scroll_y_ = event.wheel.y;
			
			//printf("mouse scroll event (%d, %d)\n", scroll_x_, scroll_y_);
			break;
		case SDL_KEYDOWN:
			is_key_pressed_[SDL_GetKeyName(event.key.keysym.sym)] = true;
			//printf("key %s scancode %s was pressed\n",
				//SDL_GetKeyName(event.key.keysym.sym),
				//SDL_GetScancodeName(event.key.keysym.scancode));
			break;
		case SDL_KEYUP:
			is_key_lifted_[SDL_GetKeyName(event.key.keysym.sym)] = true;
			//printf("key %s scancode %s was lifted\n",
				//SDL_GetKeyName(event.key.keysym.sym),
				//SDL_GetScancodeName(event.key.keysym.scancode));
			break;
		case SDL_TEXTINPUT:
			strcat(text_input_, event.text.text);
			//printf("received textinput event, text_input_ = %s\n", text_input_);
			break;
		case SDL_TEXTEDITING:
			//printf("received textediting event\n");
			break;
		}
	}
}

#else

//collect everything from the event queue
void PWEngine::poll_all_events(){
	SDL_Event event;
	SDL_Keycode keycode;
	
	//read all queued input events
	while(SDL_PollEvent(&event)){
		switch(event.type){
		case SDL_QUIT:
			is_quit_ = true;
			printf("window X button pressed\n");
			break;
		case SDL_MOUSEMOTION:
			mouse_x_ = event.motion.x;
			mouse_y_ = event.motion.y;
			printf("mouse at (%d, %d)\n", mouse_x_, mouse_y_);
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch(event.button.button){
			case SDL_BUTTON_LEFT:
				is_left_button_pressed_ = true;
				printf("mouse left button pressed\n");
				break;
			case SDL_BUTTON_RIGHT:
				is_right_button_pressed_ = true;
				printf("mouse right button pressed\n");
				break;
			case SDL_BUTTON_MIDDLE:
				is_middle_button_pressed_ = true;
				printf("mouse middle button pressed\n");
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch(event.button.button){
			case SDL_BUTTON_LEFT:
				is_left_button_lifted_ = true;
				printf("mouse left button lifted\n");
				break;
			case SDL_BUTTON_RIGHT:
				is_right_button_lifted_ = true;
				printf("mouse right button lifted\n");
				break;
			case SDL_BUTTON_MIDDLE:
				is_middle_button_lifted_ = true;
				printf("mouse middle button lifted\n");
				break;
			}
			break;
		case SDL_MOUSEWHEEL:
			scroll_x_ = event.wheel.x;
			scroll_y_ = event.wheel.y;
			
			printf("mouse scroll event (%d, %d)\n", scroll_x_, scroll_y_);
			break;
		case SDL_KEYDOWN:
			is_key_pressed_[SDL_GetKeyName(event.key.keysym.sym)] = true;
			printf("key %s scancode %s was pressed\n",
				SDL_GetKeyName(event.key.keysym.sym),
				SDL_GetScancodeName(event.key.keysym.scancode));
			break;
		case SDL_KEYUP:
			is_key_lifted_[SDL_GetKeyName(event.key.keysym.sym)] = true;
			printf("key %s scancode %s was lifted\n",
				SDL_GetKeyName(event.key.keysym.sym),
				SDL_GetScancodeName(event.key.keysym.scancode));
			break;
		case SDL_TEXTINPUT:
			strcat(text_input_, event.text.text);
			printf("received textinput event, text_input_ = %s\n", text_input_);
			break;
		case SDL_TEXTEDITING:
			//printf("received textediting event\n");
			break;
		}
	}
}
	
#endif

//calls update() for all active objects - destroys objects set to be
//destroyed
void PWEngine::update_all(){
	std::map<int, PWObject*>::iterator it;
	std::map<std::string, bool>::iterator it2;
	PWObject *object;
	
	deltatime_ = next_sleep_time_ - last_sleep_time_;
	last_update_time_ = last_sleep_time_;
	
	//destroy any objects waiting to be destroyed.
	it = objects_.begin();
	while(it != objects_.end()){
		object = it->second;
		
		if(object && object->is_destroyed()){
			delete object;
			objects_.erase(it++);
		}
		else{
			it++;
		}
	}
	
	//call update() for all objects
	it = objects_.begin();
	for(it = objects_.begin(); it != objects_.end(); ++it){
		object = it->second;
		
		if(object->is_active()){
			object->update();
		}
	}
	
	//clear events
	is_left_button_pressed_ = false;
	is_right_button_pressed_ = false;
	is_middle_button_pressed_ = false;
	
	is_left_button_lifted_ = false;
	is_right_button_lifted_ = false;
	is_middle_button_lifted_ = false;
	
	for(it2 = is_key_pressed_.begin(); it2 != is_key_pressed_.end(); ++it2){
		it2->second = false;
	}
	for(it2 = is_key_lifted_.begin(); it2 != is_key_lifted_.end(); ++it2){
		it2->second = false;
	}
	
	scroll_x_ = 0;
	scroll_y_ = 0;
	
	text_input_[0] =  '\0';
}

//time difference in milliseconds since last update
int PWEngine::deltatime(){
	return deltatime_;
}

//number of game ticks since the start of the program
int PWEngine::ticks(){
	return ticks_;
}

//draw to specified layer number - lower numbers take higher precedence.
void PWEngine::draw(SDL_Texture *src, SDL_Rect *src_rect, SDL_Rect *dst_rect, int layer_num){
	SDL_Rect d_rect, s_rect;
	if(layer_num < 10){
		//null source rectangle indicates the whole surface being blitted
		if(!src_rect){
			s_rect.x = 0;
			s_rect.y = 0;
			s_rect.w = window_surface_->w;
			s_rect.h = window_surface_->h;
		}
		else{
			s_rect = *src_rect;
		}
		
		//if destination rect is null
		if(!dst_rect){
			d_rect.x = 0;
			d_rect.y = 0;
			
			//get the dimensions of the texture
			SDL_QueryTexture(src, NULL, NULL, &(d_rect.w), &(d_rect.h));
		}
		else{
			d_rect = *dst_rect;
		}
		
		if(dst_rect->w == 0 || dst_rect->h == 0){
			//get the dimensions of the texture
			SDL_QueryTexture(src, NULL, NULL, &(d_rect.w), &(d_rect.h));
		}
		
		layer_texture_[layer_num].push_back(src);
		layer_src_rect_[layer_num].push_back(s_rect);
		layer_dst_rect_[layer_num].push_back(d_rect);
	}
}

void PWEngine::draw(PWSprite *sprite, int x, int y, int layer_num){
	SDL_Rect d_rect, s_rect;
	int start_time;
	int period;
	int num_frames;
	
	if(!sprite || layer_num >= 10){
		return;
	}
	
	start_time = sprite->start_time;
	period = sprite->frame_length;
	num_frames = sprite->num_frames;
	
	//let s_rect be the section of the texture we are drawing
	s_rect.x = sprite->x_origin;
	s_rect.y = sprite->y_origin
		+ sprite->h_frame * (((ticks() - start_time) / period) % num_frames);
	
	s_rect.w = sprite->w_frame;
	s_rect.h = sprite->h_frame;
	
	d_rect.x = x;
	d_rect.y = y;
	d_rect.w = sprite->w_frame;
	d_rect.h = sprite->h_frame;
	
	//queue the sprite to render
	layer_texture_[layer_num].push_back(sprite->texture);
	layer_src_rect_[layer_num].push_back(s_rect);
	layer_dst_rect_[layer_num].push_back(d_rect);
}

//--------------
//rendering text

const char CHARSET_W[128] = 
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /*0x00*/
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /*0x10*/
	2,2,4,8,6,10,9,2,4,4,6,6,2,6,2,6, /*0x20*/
	6,6,6,6,6,6,6,6,6,6,2,2,5,6,5,6, /*0x30*/
	10,8,7,8,8,7,7,8,7,6,9,7,7,8,8,8, /*0x40*/
	7,9,7,7,8,8,8,10,8,8,8,4,6,4,6,7, /*0x50*/
	3,6,6,5,6,6,6,6,6,2,5,6,2,8,6,6, /*0x60*/
	6,6,5,6,4,6,6,10,6,6,6,4,2,4,8,0 /*0x70*/
};

//default height is 12 pixels
#define CHARSET_H 12
#define TAB_WIDTH 50
#define SPACE_WIDTH 4

//draw string that wraps around the square
int PWEngine::draw_str_wrapped(const char *src, int x, int y, int w, int h, int len, int layer_num){
	int result = 0;
	SDL_Rect default_offset;
	SDL_Rect charset_clip;
	SDL_Rect dest_clip;
	int i;
	
	//for word lengths
	size_t word_length;
	size_t word_pixel_width;
	size_t str_offset = 0;
	
	//where on the destination to paste
	dest_clip.x = x;
	dest_clip.y = y;
	dest_clip.h = CHARSET_H;
	
	//where on the charset map to copy
	charset_clip.h = CHARSET_H;
	
	/*draw the string*/
	while(str_offset < strlen(src) && str_offset < len){
		/*draw whitespace*/
		for(i = 0; (i < strspn(src + str_offset, " \n\r\t\x01\x02")) && (str_offset + i < len); ++i){
			switch(src[str_offset + i]){
			case '\n': 
				dest_clip.y += CHARSET_H;
				break;
			case '\r':
				dest_clip.x = x;
				break;
			case '\t':
				dest_clip.x = ((dest_clip.x - x) / 50) * 50 + 50 + x;
				break;
			case ' ':
				dest_clip.x += CHARSET_W[' '] + 2;
				break;
			case 0x01: //subscript
				dest_clip.y += CHARSET_H / 2;
				break;
			case 0x02: //superscript
				dest_clip.y -= CHARSET_H / 2;
				break;
			default:
				break;
			}
			
			/*do any necessary wrap-arounds*/
			if(dest_clip.x - x >= w){
				dest_clip.x = x;
				dest_clip.y += CHARSET_H;
			}
			
			/*if it has reached beyond the bottom of the destination clip*/
			if(dest_clip.y - y >= h){
				break;
			}
		}
		str_offset += strspn(src + str_offset, " \n\r\t\x01\x02");
		
		/*individual word*/
		word_pixel_width = 0;
		for(i = 0; i < strcspn(src + str_offset, " \n\r\t\x01\x02") && str_offset + i < len; ++i){
			word_pixel_width += CHARSET_W[src[str_offset + i]] + 1;
		}
		
		/*go to the next line if the next word is too big to fit in current line*/
		//printf("word_pixel_width = %d, dest_clip.x = %d, w = %d\n", word_pixel_width, dest_clip.x, w);
		//if(word_pixel_width > w - dest_clip.x){
		if(dest_clip.x - x + word_pixel_width >= w && dest_clip.x != x){
			dest_clip.x = x;
			dest_clip.y += 12;
		}
		/*if it has reached beyond the bottom of the destination clip*/
		if(dest_clip.y - y >= h){
			break;
		}
		
		/*draw the word*/
		for(i = 0; (i < strcspn(src + str_offset, " \n\r\t\x01\x02")) && (str_offset + i < len); ++i){
			/*get the dimension of the current character*/
			charset_clip.w = dest_clip.w = CHARSET_W[src[str_offset + i]] + 1;
			
			/*get the offset of the character on the sprite sheet*/
			charset_clip.x = 12 * (src[str_offset + i] % 16);
			charset_clip.y = 12 * (src[str_offset + i] / 16);
			
			/*draw the character*/
			draw(charset_, &charset_clip, &dest_clip, layer_num);
			dest_clip.x += charset_clip.w;
			
			//if the word is longer than the full width of the box
			if(dest_clip.x - x > w){
				dest_clip.x = x;
				dest_clip.y += 12;
			}
			//printf("offset x,y : %d,%d\n", dest_clip.x - x, dest_clip.y - y);
			//printf("dest x,y,w,h : %d,%d,%d,%d\n", dest_clip.x, dest_clip.y, dest_clip.w, dest_clip.h);
		}
		str_offset += strcspn(src + str_offset, " \n\r\t\x01\x02");
	}
	
	return 0;
}

//draw to current window
void PWEngine::render(){
	int i, ii;
	SDL_Texture *src;
	SDL_Rect src_rect, dst_rect;
	
	//clear screen
	SDL_RenderClear(renderer_);
	
	//draw all layers on the window layer
	for(i = 0; i < 10; ++i){
		for(ii = 0; ii < layer_texture_[10 - 1 - i].size(); ++ii){
			src = layer_texture_[10 - 1 - i][ii];
			src_rect = layer_src_rect_[10 - 1 - i][ii];
			dst_rect = layer_dst_rect_[10 - 1 - i][ii];
			
			SDL_RenderCopy(renderer_, src, &src_rect, &dst_rect);
		}
	}
	
	//update screen
	SDL_RenderPresent(renderer_);
	
	//clear all layers
	for(i = 0; i < 10; ++i){
		layer_texture_[i].clear();
		layer_src_rect_[i].clear();
		layer_dst_rect_[i].clear();
	}
}

void PWEngine::sleep_until_next_frame(){
	int current_time;
	
	#if (defined(unix) || defined(__unix) || defined(__unix__))
	struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC, &tp);
	
	current_time = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
	#endif
	
	#if defined(_WIN32)
	current_time = clock() / (CLOCKS_PER_SEC / 1000);
	#endif
	
	if(current_time < next_sleep_time_){
		//linux specific
		#if (defined(unix) || defined(__unix) || defined(__unix__))
		usleep((next_sleep_time_ - current_time) * 1000);
		#endif

		//windows specific
		#if defined(_WIN32)
		Sleep(next_sleep_time_ - current_time);
		#endif
	}
	
	last_sleep_time_ = next_sleep_time_;
	next_sleep_time_ = last_sleep_time_ + (1000 / framerate_);
	
	ticks_++;
}

//Implementation is very much the same as Lazyfoo SDL 2's
SDL_Texture *PWEngine::load_image(std::string path){
	//The final texture
	SDL_Texture *new_texture = NULL;

	//Load image at specified path
	SDL_Surface* loaded_surface = NULL;
	
	if(loaded_textures_.find(path) != loaded_textures_.end()){
		new_texture = loaded_textures_[path];
	}
	else{
		loaded_surface = IMG_Load(path.c_str());
		
		if(loaded_surface == NULL){
			printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		}
		else{
			//set colour key to black
			SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0x00, 0x00, 0x00));
			
			new_texture = SDL_CreateTextureFromSurface(renderer_, loaded_surface);
			
			if(new_texture == NULL){
				printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
			}
			
			//Get rid of old loaded surface
			SDL_FreeSurface(loaded_surface);
			
			loaded_textures_[path] = new_texture;
		}
	}

	return new_texture;
}


//Returns a newly allocated sprite struct, or nullptr if not.
//Use delete operator on sprite to deallocate.
PWSprite *PWEngine::load_sprite(
	std::string path,
	int w,
	int h,
	int x,
	int y,
	int num_frames,
	int period
){
	int w_texture;
	int h_texture;
	PWSprite *sprite = new PWSprite;
	
	sprite->x_origin = x;
	sprite->y_origin = y;
	/*
	sprite->w_frame = w;
	sprite->h_frame = h;
	*/
	sprite->num_frames = num_frames;
	
	//set the start time to the number of game ticks since the program started
	sprite->start_time = ticks();
	sprite->frame_length = period;
	
	//get texture from the engine itself
	sprite->texture = load_image(path);
	
	//get info from the texture
	SDL_QueryTexture(sprite->texture, NULL, NULL, &w_texture, &h_texture);
	
	//TODO: give a warning if incorrect dimensions are given
	//1. x or y is greater than image boundary or less than 0
	//2. width or height exceeds image boundary
	//3. number of frames is less than 1
	//4. number of frames goes beyond what the image height allows
	
	if(w > w_texture || w < 0){
		sprite->w_frame = w_texture;
	}
	else{
		sprite->w_frame = w;
	}
	if(h > h_texture || h < 0){
		sprite->h_frame = h_texture;
	}
	else{
		sprite->h_frame = h;
	}
	
	return sprite;
}

int PWEngine::get_mouse_x(){
	return mouse_x_;
}

int PWEngine::get_mouse_y(){
	return mouse_y_;
}

//input events
bool PWEngine::is_left_button_pressed(){
	return is_left_button_pressed_;
}

bool PWEngine::is_right_button_pressed(){
	return is_right_button_pressed_;
}

bool PWEngine::is_middle_button_pressed(){
	return is_middle_button_pressed_;
}

bool PWEngine::is_left_button_lifted(){
	return is_left_button_lifted_;
}

bool PWEngine::is_right_button_lifted(){
	return is_right_button_lifted_;
}

bool PWEngine::is_middle_button_lifted(){
	return is_middle_button_lifted_;
}

bool PWEngine::is_key_pressed(std::string key){
	return is_key_pressed_[key];
}

bool PWEngine::is_key_lifted(std::string key){
	return is_key_lifted_[key];
}

Space PWEngine::get_arrow_direction(){
	float x, y;
	
	//determine direction
	if(is_key_pressed_["Right"]){
		direction_ |= 0x1;
	}
	if(is_key_pressed_["Up"]){
		direction_ |= 0x2;
	}
	if(is_key_pressed_["Left"]){
		direction_ |= 0x4;
	}
	if(is_key_pressed_["Down"]){
		direction_ |= 0x8;
	}
	if(is_key_lifted_["Right"]){
		direction_ &= 0xe;
	}
	if(is_key_lifted_["Up"]){
		direction_ &= 0xd;
	}
	if(is_key_lifted_["Left"]){
		direction_ &= 0xb;
	}
	if(is_key_lifted_["Down"]){
		direction_ &= 0x7;
	}
	
	//printf("arrow direction = %x ", direction_);
	
	switch(direction_){
	case 0x1: //right
		x = 1.0;
		y = 0.0;
		break;
	case 0x3: //top right
		x = 0.71;
		y = -0.71;
		break;
	case 0x2: //up
		x = 0.0;
		y = -1.0;
		break;
	case 0x6: //top left
		x = -0.71;
		y = -0.71;
		break;
	case 0x4: //left
		x = -1.0;
		y = 0.0;
		break;
	case 0xc: //bottom left
		x = -0.71;
		y = 0.71;
		break;
	case 0x8: //down
		x = 0.0;
		y = 1.0;
		break;
	case 0x9: //bottom right
		x = 0.71;
		y = 0.71;
		break;
	default:
		x = 0.0;
		y = 0.0;
		break;
	}
	
	return Space(x, y, 0, 0);
}

const char *PWEngine::get_text_input(){
	return text_input_;
}

bool PWEngine::is_quit(){
	return is_quit_;
}
