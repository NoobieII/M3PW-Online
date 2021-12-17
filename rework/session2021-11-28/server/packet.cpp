#include "packet.hpp"

#include <cstring>
#include <string>

Packet::Packet():
	bytes_used_(0),
	read_length_(0),
	buffer_size_(PACKET_START_SIZE),
	buffer_(new char[PACKET_START_SIZE])
{
}

Packet::~Packet(){
	delete[] buffer_;
}

void Packet::send_ivs(const unsigned char *send_iv, const unsigned char *recv_iv){
	write<short>(32767);
	write_bytes(send_iv, 16);
	write_bytes(recv_iv, 16);
}

void Packet::test_message(const char *message){
	write<short>(32766);
	write<const char*>(message);
}

void Packet::reset(){
	bytes_used_ = 0;
	read_length_ = 0;
}

char *Packet::get_buffer(){
	return buffer_;
}

int Packet::get_packet_length(){
	return bytes_used_;
}

//template specialization for bool to make sure it uses only one byte
template <>
void Packet::write<bool>(bool value){
	if(value == false){
		write<char>(0);
	}
	else{
		write<char>(1);
	}
}

//template specialization for c string types
template <>
void Packet::write<const char*>(const char *value){
	char *tmp_buffer;
	int string_length;
	
	//length of the string including null character
	string_length = strlen(value) + 1;
	
	//write the length of the string including null character
	write<short>(string_length);
	
	//check if the buffer needs to be resized
	if(bytes_used_ + string_length > buffer_size_){
		tmp_buffer = buffer_;
		buffer_size_ += PACKET_INCREASE_SIZE;
		buffer_ = new char[buffer_size_];
		memcpy(buffer_, tmp_buffer, bytes_used_);
		delete[] tmp_buffer;
		tmp_buffer = nullptr;
		
	}
	
	//write to the buffer directly
	memcpy(buffer_ + bytes_used_, value, string_length);
	
	//add to the bytes used
	bytes_used_ += string_length;
}

template <>
void Packet::write<std::string>(std::string value){
	write<const char*>(value.c_str());
}



//template specialization for bool types
template <>
bool Packet::read<bool>(){
	return read<char>();
}

//template specialization for c string type - be careful using this
template <>
const char *Packet::read<const char*>(){
	short string_length;
	const char *val;
	
	string_length = read<short>();
	val = buffer_ + read_length_;
	read_length_ += string_length;
	
	return val;
}

//template specialization for string type
template <>
std::string Packet::read<std::string>(){
	std::string val;
	val = read<const char*>();
	return val;
}

void Packet::write_bytes(const void *val, size_t num_bytes){
	char *tmp_buffer;
	
	//check if the buffer needs to be resized
	if(bytes_used_ + num_bytes > buffer_size_){
		tmp_buffer = buffer_;
		buffer_size_ += num_bytes/*PACKET_INCREASE_SIZE*/;
		buffer_ = new char[buffer_size_];
		memcpy(buffer_, tmp_buffer, bytes_used_);
		delete[] tmp_buffer;
		tmp_buffer = nullptr;	
	}
	
	//copy the contents from val
	memcpy(buffer_ + bytes_used_, val, num_bytes);
	bytes_used_ += num_bytes;
}
