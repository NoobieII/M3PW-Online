#include "textbox.hpp"
#include <string.h>

TextBox::TextBox(int size):
size_(size),
pos_(0),
len_(0),
str_(0)
{
	str_ = new char[size_]();
}

TextBox::~TextBox(){
	delete[] str_;
}

void TextBox::backspace(){
	if(pos_ == 0){
		return;
	}
	
	str_[pos_ - 1] = 0;
	
	//if there is nothing in front of the cursor to move back one space
	if(pos_-- == len_--){
		return;
	}
	
	//copy what is in front of the cursor back one space
	memcpy(str_ + pos_, str_ + pos_ + 1, len_ - pos_);
	
	//remove the end of the string
	str_[len_] = 0;
}

void TextBox::del(){
	if(pos_ == len_){
		return;
	}
	
	str_[pos_] = 0;
	len_--;
	
	//if the most front character has been deleted
	if(pos_ == len_){
		return;
	}
	//copy the front of the string back one space
	memcpy(str_ + pos_, str_ + pos_ + 1, len_ - pos_);
	
	//remove the end of the string
	str_[len_] = 0;
}
	
void TextBox::add_char(char ch){
	//do not add a char if length of string is maximum
	if(len_ == size_ - 1){
		return;
	}
	
	//copy memory to one spot forward if position is not length
	if(pos_ < len_){
		memcpy(str_ + pos_ + 1, str_ + pos_, len_ - pos_);
	}
	str_[pos_] = ch;
	pos_++;
	len_++;
}
void TextBox::move_right(){
	if(pos_ < len_){
		pos_++;
	}
}

void TextBox::move_left(){
	if(pos_ > 0){
		pos_--;
	}
}

void TextBox::replace(const char *str){
	strncpy(str_, str, size_ - 1);
	pos_ = strlen(str_);
	len_ = pos_;
}
	
int TextBox::get_pos(){
	return pos_;
}

const char* TextBox::get_str(){
	return str_;
}

void TextBox::clear(){
	memset(str_, 0, size_);
	len_ = 0;
	pos_ = 0;
}

void TextBox::resize(int size){
	char *tmp = nullptr;
	
	if(size < 1){
		return;
	}
	
	tmp = str_;
	str_ = new char[size]();
	
	//if amount of characters filled in is greater than the maximum size of new buffer,
	if(len_ > size - 1){
		memcpy(str_, tmp, size - 1);
		len_ = size - 1;
	}
	else{
		memcpy(str_, tmp, len_);
	}
	size_ = size;
	
	//reposition
	if(pos_ > size - 1){
		pos_ = size - 1;
	}
}
