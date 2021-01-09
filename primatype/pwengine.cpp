//Lithio
//December 18, 2020

//for clock()
#include <ctime>
#include <cstdio> //for printf
#include <cstring> //for strlen

#include <SDL_image.h>

#include "pwengine.hpp"
#include "pwobject.hpp"

PWEngine::PWEngine():
deltatime_(0),
framerate_(0),
last_update_time_(0),
last_sleep_time_(0),
next_sleep_time_(0),
object_id_(0),
objects_(),
object_name_id_(),
window_(nullptr),
window_surface_(nullptr),
renderer_(nullptr),
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
	
	SDL_SetRenderDrawColor(renderer_, 0xff, 0xff, 0xff, 0xff);
	
	framerate_ = framerate;
	last_update_time_ = clock() / (CLOCKS_PER_SEC / 1000);
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
		}
		else{
			d_rect = *dst_rect;
		}
		//get the dimensions of the texture
		SDL_QueryTexture(src, NULL, NULL, &(d_rect.w), &(d_rect.h));
		
		layer_texture_[layer_num].push_back(src);
		layer_src_rect_[layer_num].push_back(s_rect);
		layer_dst_rect_[layer_num].push_back(d_rect);
	}
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
	//if the current time is less than the time for next frame
	int current_time = clock() / (CLOCKS_PER_SEC / 1000);
	
	if(current_time < next_sleep_time_){
		SDL_Delay(next_sleep_time_ - current_time);
	}
	
	last_sleep_time_ = next_sleep_time_;
	next_sleep_time_ = last_sleep_time_ + (1000 / framerate_);
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

const char *PWEngine::get_text_input(){
	return text_input_;
}

bool PWEngine::is_quit(){
	return is_quit_;
}
