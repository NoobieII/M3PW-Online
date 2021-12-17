//Lithio
//October 08, 2021
//
//Engine class header file

#ifndef ENGINE_HPP
#define ENGINE_HPP

#define DEFAULT_BUFLEN 256

#include <string>

#include <windows.h>

class Packet;

class Engine{
public:
	//default constructor
	Engine();
	
	//window dimensions, framerate and caption. Returns 0 if successful,
	//returns -1 if failed
	int initialize(int width, int height, int framerate, const char *caption);
	
	//attempt TCP connection to host and port. Returns 0 if successful,
	//returns -1 if failed.
	int connect(const char *host, short port);
	
	//sends packet-
	//returns -1 if failed, returns 0 if successful.
	int send_packet(Packet *p);
	
	//checks if the socket is readable and copy all received bytes to recv_
	//returns -1 if failed, returns 0 if successful.
	int poll_recv_packets();
	
	//puts the thread to sleep until the next frame
	void sleep_until_next_frame();
	
	//polls the socket (if enabled)
	void poll_all_events();
	
	//handles data in the receive buffer (if socket enabled),
	//writes data to the send buffer,
	//sends data in the send buffer (if socket enabled)
	void update_all();
	
	//destructor
	~Engine();
private:
	//disable copy constructor usage
	Engine(const Engine &e);
	
	//number of milliseconds between frames, frames per second, number of
	//frames since beginning of thread, last time updated, last time slept,
	//next sleep time
	int deltatime_;
	int framerate_;
	int ticks_;
	int last_update_time_;
	int last_sleep_time_;
	int next_sleep_time_;
	
	//is socket enabled (0/1), size of allocated buffer, length of used buffer,
	//buffer location
	int sock_enabled_;
	int send_size_;
	int send_length_;
	char *send_;
	int recv_size_;
	int recv_length_;
	int read_length_;
	char *recv_;
	
	//socket class (OS dependant)
	//--Windows--
	SOCKET socket_;
	
	//generic template for reading integer types
	template <typename T>
	T read(){
		T val;
		
		val = *(T*)(recv_ + read_length_);
		read_length_ += sizeof(T);
		
		return val;
	}
};

template <>
bool Engine::read<bool>();
template <>
const char *Engine::read<const char*>();
template <>
std::string Engine::read<std::string>();

#endif
