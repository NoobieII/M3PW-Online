//Lithio
//Late 2019, likely

//Class for creating packets
//Usage:
//Packet packet;
//packet.name_your_packet(param1, param2...);
//
//Use the given functions, and your own created functions to create packets.
//First 2 bytes in the packet is the header, every byte after it is the data.

#ifndef PACKET_HPP
#define PACKET_HPP

#define PACKET_START_SIZE    16
#define PACKET_INCREASE_SIZE 64

#include <string>
#include <cstring>
#include <list>

class Packet{
public:
	Packet();
	~Packet();
	
	//DEFAULT PACKETS
	//16 byte iv's for each (client use only)
	void send_ivs(const unsigned char *send_iv, const unsigned char *recv_iv);
	//test send message (client and server use)
	void test_message(const char *message);
	
	
	//delete the contents of the buffer to reuse the packet
	void reset();
	
	//Session::send() needs access to the buffer in order to send it
	//and free memory once it is sent
	char *get_buffer();
	
	//returns bytes_used_ - length of packet body + 2
	int get_packet_length();
	
	//template for writing values
	//this generic template is for integer types only
	template <typename T>
	void write(T value){
		char *tmp_buffer;
		
		//check if the buffer needs to be resized
		if(bytes_used_ + sizeof(T) > buffer_size_){
			tmp_buffer = buffer_;
			buffer_size_ += PACKET_INCREASE_SIZE;
			buffer_ = new char[buffer_size_];
			memcpy(buffer_, tmp_buffer, bytes_used_);
			delete[] tmp_buffer;
			tmp_buffer = nullptr;
			
		}
		
		//write to the buffer directly
		*(T*)(buffer_ + bytes_used_) = value;
		
		//add to the bytes used
		bytes_used_ += sizeof(T);
	}
	
	//for reading - debugging purposes
	//generic template for reading integer types
	template <typename T>
	T read(){
		T val;
		
		val = *(T*)(buffer_ + read_length_);
		read_length_ += sizeof(T);
		
		return val;
	}
	
	void write_bytes(const void *val, size_t num_bytes);
private:
	int bytes_used_;
	int read_length_;
	int buffer_size_;
	char *buffer_;
};

template <>
void Packet::write<bool>(bool value);
template <>
void Packet::write<const char*>(const char *value);
template <>
void Packet::write<std::string>(std::string value);

template <>
bool Packet::read<bool>();
template <>
const char *Packet::read<const char*>();
template <>
std::string Packet::read<std::string>();

#endif