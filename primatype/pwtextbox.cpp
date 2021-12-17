//Lithio
//June 10, 2021

//implementation of textbox, a lot copied from old code

#include <cstring>

#include "pwengine.hpp"
#include "pwtextbox.hpp"

int get_offset_x(const char *src, int w, int h, int len);
int get_offset_y(const char *src, int w, int h, int len);
int get_pos_from_mouse(const char *src, int x, int y, int w, int h, int len);

PWTextbox::PWTextbox(
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
):
PWObject(name, engine, true),
x_(x),
y_(y),
w_(w),
h_(h),
size_(size),
sprite_layer_(sprite_layer),
accept_newlines_(accept_newlines),
censored_(censored),
underlay_(underlay),
pos_(0),
len_(0),
state_(0),
recalculate_cursor_position_(false),
cursor_x_(x),
cursor_y_(y)
{
	str_ = new char[size]();
	str_censored_ = new char[size]();
	
	//make the "censored" string all asterisks
	memset(str_censored_, '*', size - 1);
	
	//load blinking sprite
	cursor_sprite_ = engine_->load_sprite(
		"sprites/text_cursor.png",
		1,
		12,
		0,
		0,
		2,
		10
	);
}


//destructor
PWTextbox::~PWTextbox(){
	delete str_;
	delete str_censored_;
	delete cursor_sprite_;
}

//called once every game tick
void PWTextbox::update(){
	int mouse_x;
	int mouse_y;
	bool is_mouse_hovered_over;
	
	//text read from keyboard
	const char *text_input;
	
	//variables for binary search
	int l, r;
	
	int i;
	
	mouse_x = engine_->get_mouse_x();
	mouse_y = engine_->get_mouse_y();
	
	is_mouse_hovered_over = mouse_x >= x_ && mouse_x < x_ + w_
		&& mouse_y >= y_ && mouse_y < y_ + h_;
	
	
	//update state machine
	switch(state_){
	//inactive state
	case 0:
		if(is_mouse_hovered_over && engine_->is_left_button_pressed()){
			state_ = 1;
		}
		break;
	//reading text input state
	case 1:
		if(!is_mouse_hovered_over && engine_->is_left_button_pressed()){
			state_ = 0;
		}
		else if(engine_->is_key_pressed("Return") && !accept_newlines_){
			state_ = 2;
		}
		break;
	//enter was pressed - return back to text input state
	case 2:
		state_ = 1;
		break;
	}
	
	//execute code depending on state
	switch(state_){
	//text box is not clicked on and therefore does not read input
	case 0:
		//do nothing
		break;
	//text box is reading input
	case 1:
		//if the arrow key (left or right) is pressed
		if(engine_->is_key_pressed("Left")){
			if(pos_ > 0){
				pos_--;
				recalculate_cursor_position_ = true;
			}
		}
		if(engine_->is_key_pressed("Right")){
			if(pos_ < len_){
				pos_++;
				recalculate_cursor_position_ = true;
			}
		}
		
		//if backspace is pressed
		if(engine_->is_key_pressed("Backspace") && pos_ != 0){
			str_[pos_ - 1] = 0;
			
			//if there is nothing in front of the cursor to move back one space
			if(pos_-- != len_--){
			
				//copy what is in front of the cursor back one space
				memcpy(str_ + pos_, str_ + pos_ + 1, len_ - pos_);
				
				//remove the end of the string
				str_[len_] = 0;
			}
			
			recalculate_cursor_position_ = true;
		}
		
		//if delete is pressed
		if(engine_->is_key_pressed("Delete") && pos_ != len_){
			
			str_[pos_] = 0;
			len_--;
			
			//if the most front character has been deleted
			if(pos_ != len_){
				
				//copy the front of the string back one space
				memcpy(str_ + pos_, str_ + pos_ + 1, len_ - pos_);
				
				//remove the end of the string
				str_[len_] = 0;
			}
			
			recalculate_cursor_position_ = true;
		}
		
		//any text passed to the textbox
		text_input = engine_->get_text_input();
		i = 0;
		while((len_ != size_ - 1) && text_input[i]){
			//copy memory to one spot forward if position is not length
			if(pos_ < len_){
				memcpy(str_ + pos_ + 1, str_ + pos_, len_ - pos_);
			}
			str_[pos_] = text_input[i];
			pos_++;
			len_++;
			
			i++;
			
			recalculate_cursor_position_ = true;
		}
		
		//TODO: if left clicking, move the cursor to the mouse cursor
		if(engine_->is_left_button_pressed()){
			if(censored_){
				pos_ = get_pos_from_mouse(str_censored_, mouse_x - x_, mouse_y - y_, w_, h_, len_);
			}
			else{
				pos_ = get_pos_from_mouse(str_, mouse_x - x_, mouse_y - y_, w_, h_, len_);
			}
			recalculate_cursor_position_ = true;
		}
		//TODO: left clicking if the text is censored
		
		break;
	}
	
	//render
	
	//show the underlay
	engine_->draw(underlay_, x_, y_, sprite_layer_);
	
	//show the text, either in censored mode or not censored
	if(censored_){
		engine_->draw_str_wrapped(str_censored_, x_, y_, w_, h_, len_, sprite_layer_);
	}
	else{
		engine_->draw_str_wrapped(str_, x_, y_, w_, h_, len_, sprite_layer_);
	}
	
	
	if(state_ == 1){
		if(recalculate_cursor_position_){
			if(censored_){
				cursor_x_ = x_ + get_offset_x(str_censored_, w_, h_, pos_);
				cursor_y_ = y_ + get_offset_y(str_censored_, w_, h_, pos_);
			}
			else{
				cursor_x_ = x_ + get_offset_x(str_, w_, h_, pos_);
				cursor_y_ = y_ + get_offset_y(str_, w_, h_, pos_);
			}
			
			recalculate_cursor_position_ = false;
		}
		
		//show blinking cursor
		engine_->draw(cursor_sprite_, cursor_x_, cursor_y_, sprite_layer_);
	}
}

//returns the string of the textbox
const char *PWTextbox::get_str(){
	return str_;
}

//clears all the text
void PWTextbox::clear(){
	memset(str_, 0, size_);
	memset(str_censored_, 0, size_);
	len_ = 0;
	pos_ = 0;
}
	
void PWTextbox::set_active(bool state){
	state_ = (int)(state);
}

//nitty gritty functions for getting the offset at a point in a string
/*default height is 12 pixels*/
#define CHARSET_H 12
#define TAB_WIDTH 50
#define SPACE_WIDTH 4

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

int get_offset_x(const char *src, int w, int h, int len){
	size_t i = 0;
	size_t str_offset = 0;
	size_t word_pixel_width = 0;
	size_t x = 0;
	size_t y = 0;
	
	while(str_offset < strlen(src) && str_offset < len){
		//skip whitespace
		for(i = 0; i < strspn(src + str_offset, " \n\r\t\x01\x02") && str_offset + i < len; ++i){
			switch(src[str_offset + i]){
			case '\n':
				y += CHARSET_H;
				break;
			case '\r':
				x = 0;
				break;
			case '\t':
				x = (x / TAB_WIDTH) * TAB_WIDTH + TAB_WIDTH;
				break;
			case ' ':
				x += CHARSET_W[' '] + 2;
				break;
			case 0x01:// subscript
				y += CHARSET_H / 2;
				break;
			case 0x02:// superscript
				y -= CHARSET_H / 2;
				break;
			default:
				break;
			}
			
			if(x >= w){
				x = 0;
				y += CHARSET_H;
			}
			if(y >= h){
				return x;
			}
		}
		str_offset += strspn(src + str_offset, " \n\r\t\x01\x02");
		
		//get word length
		
		word_pixel_width = 0;
		for(i = 0; i < strcspn(src + str_offset, " \n\r\t\x01\x02") /*&& str_offset + i < len*/; ++i){
			word_pixel_width += CHARSET_W[src[str_offset + i]] + 1;
		}
		if(x + word_pixel_width >= w && x != 0){
			x = 0;
			y += CHARSET_H;
		}
		
		//if the word is longer than the full width of the box
		for(i = 0; (i < strcspn(src + str_offset, " \n\r\t\x01\x02")) && (str_offset + i < len); ++i){
			x += CHARSET_W[src[str_offset + i]] + 1;
			
			//if the word is longer than the full width of the box
			if(x > w){
				x = 0;
				y += CHARSET_H;
			}
		}
		//x += word_pixel_width;
		str_offset += strcspn(src + str_offset, " \n\r\t\x01\x02");
	}
	//printf("offset x : %d\n", x);
	return x;
}

int get_offset_y(const char *src, int w, int h, int len){
	size_t i = 0;
	size_t str_offset = 0;
	size_t word_pixel_width = 0;
	size_t x = 0;
	size_t y = 0;
	
	while(str_offset < strlen(src) && str_offset < len){
		//skip whitespace
		for(i = 0; i < strspn(src + str_offset, " \n\r\t\x01\x02") && str_offset + i < len; ++i){
			switch(src[str_offset + i]){
			case '\n':
				y += CHARSET_H;
				break;
			case '\r':
				x = 0;
				break;
			case '\t':
				x = (x / TAB_WIDTH) * TAB_WIDTH + TAB_WIDTH;
				break;
			case ' ':
				x += CHARSET_W[' '] + 2;
				break;
			case 0x01:// subscript
				y += CHARSET_H / 2;
				break;
			case 0x02:// superscript
				y -= CHARSET_H / 2;
				break;
			default:
				break;
			}
			
			if(x >= w){
				x = 0;
				y += CHARSET_H;
			}
			if(y >= h){
				return y;
			}
		}
		str_offset += strspn(src + str_offset, " \n\r\t\x01\x02");
		
		//get word length
		
		word_pixel_width = 0;
		for(i = 0; i < strcspn(src + str_offset, " \n\r\t\x01\x02") /*&& str_offset + i < len*/; ++i){
			word_pixel_width += CHARSET_W[src[str_offset + i]] + 1;
		}
		if(x + word_pixel_width >= w && x != 0){
			x = 0;
			y += CHARSET_H;
		}
		
		//if the word is longer than the full width of the box
		for(i = 0; (i < strcspn(src + str_offset, " \n\r\t\x01\x02")) && (str_offset + i < len); ++i){
			x += CHARSET_W[src[str_offset + i]] + 1;
			
			//if the word is longer than the full width of the box
			if(x > w){
				x = 0;
				y += CHARSET_H;
			}
		}
		
		//x += word_pixel_width;
		str_offset += strcspn(src + str_offset, " \n\r\t\x01\x02");
	}
	//printf("offset y : %d\n", y);
	return y;
}

int get_pos_from_mouse(const char *src, int mouse_x, int mouse_y, int w, int h, int len){
	size_t i = 0;
	size_t str_offset = 0;
	size_t word_pixel_width = 0;
	size_t x = 0;
	size_t y = 0;
	
	//adjust the mouse x and y positions
	if(mouse_x < 0){
		mouse_x = 0;
	}
	if(mouse_x > w){
		mouse_x = w;
	}
	if(mouse_y < 0){
		return 0;
	}
	if(mouse_y > h){
		mouse_y = h / CHARSET_H * CHARSET_H;
	}
	mouse_y = mouse_y / CHARSET_H * CHARSET_H;
	
	while(str_offset < strlen(src) && str_offset < len){
		//skip whitespace
		for(i = 0; i < strspn(src + str_offset, " \n\r\t\x01\x02")
			&& str_offset + i < len; ++i){
			
			if((y == mouse_y && x >= mouse_x) || y > mouse_y){
				return str_offset + i;
			}
			
			switch(src[str_offset + i]){
			case '\n':
				y += CHARSET_H;
				break;
			case '\r':
				x = 0;
				break;
			case '\t':
				x = (x / TAB_WIDTH) * TAB_WIDTH + TAB_WIDTH;
				break;
			case ' ':
				x += CHARSET_W[' '] + 2;
				break;
			case 0x01:// subscript
				y += CHARSET_H / 2;
				break;
			case 0x02:// superscript
				y -= CHARSET_H / 2;
				break;
			default:
				break;
			}
			
			if(x >= w){
				x = 0;
				y += CHARSET_H;
			}
		}
		str_offset += strspn(src + str_offset, " \n\r\t\x01\x02");
		
		//get word length
		
		word_pixel_width = 0;
		for(i = 0; i < strcspn(src + str_offset, " \n\r\t\x01\x02") /*&& str_offset + i < len*/; ++i){
			word_pixel_width += CHARSET_W[src[str_offset + i]] + 1;
		}
		if(x + word_pixel_width >= w && x != 0){
			x = 0;
			y += CHARSET_H;
		}
		
		//if the word is longer than the full width of the box
		for(i = 0; (i < strcspn(src + str_offset, " \n\r\t\x01\x02")) && (str_offset + i < len); ++i){
			if((y == mouse_y && x >= mouse_x) || y > mouse_y){
				return str_offset + i;
			}
			
			x += CHARSET_W[src[str_offset + i]] + 1;
			
			//if the word is longer than the full width of the box
			if(x > w){
				x = 0;
				y += CHARSET_H;
			}
		}
		//x += word_pixel_width;
		str_offset += strcspn(src + str_offset, " \n\r\t\x01\x02");
	}
	//printf("offset x : %d\n", x);
	return len;
}

