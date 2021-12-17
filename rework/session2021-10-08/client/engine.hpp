//Lithio
//October 08, 2021
//
//Engine class header file

#ifndef ENGINE_HPP
#define ENGINE_HPP

#define ECB 1
#define CBC 0
#define CTR 1

#include "aes/aes.h"

#include <string>
#include <boost/asio.hpp>
// PLEASE NOTE: Boost's shared_ptr is only used for Session. Use the C++ standard
// shared_ptr everywhere else.
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class Packet;

class Engine{
public:
	//default constructor
	Engine();
	
	//destructor
	~Engine();
	
	//window dimensions, framerate and caption. Returns 0 if successful,
	//returns -1 if failed
	int initialize(int width, int height, int framerate, const char *caption);
	
	//puts the thread to sleep until the next frame
	void sleep_until_next_frame();
	
	//polls the socket (if enabled)
	void poll_all_events();
	
	//handles data in the receive buffer (if socket enabled),
	//writes data to the send buffer,
	//sends data in the send buffer (if socket enabled)
	void update_all();
	
	//Networking stuff (optional)
	
	//attempt IPv4 TCP connection to host and port. Returns 0 if successful,
	//returns -1 if failed.
	int connect(std::string address, unsigned short port);
	
	//disconnect - if socket enabled, must be called before deleting the engine
	void disconnect();
	
	//these three functions are called in the process of reading a packet,
	//and call each other in a cycle.
	void start_read_header();
	void read_header_handler(const boost::system::error_code &ec, size_t bytes_transferred);
	void read_body_handler(const boost::system::error_code &ec, size_t bytes_transferred);
	
	void send_packet(Packet *packet);
	void write_handler(const boost::system::error_code &ec, size_t bytes_transferred, char *send_buffer);
	
	//packet handlers
	//client and server use
	void handle_test_message();
	
	//generic template for reading integer types
	template <typename T>
	T read(){
		T val;
		
		val = *(T*)(recv_ + read_length_);
		read_length_ += sizeof(T);
		
		return val;
	}
	
	void skip_bytes(size_t n);
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
	
	//Networking stuff for socket implementation
	bool socket_enabled_;
	size_t read_length_;
	boost::asio::io_service io_;
	boost::asio::ip::tcp::socket socket_;
	unsigned char *iv_send_;
	unsigned char *iv_recv_;
	AES_ctx send_ctx_;
	AES_ctx recv_ctx_;
	char *recv_;
};

template <>
bool Engine::read<bool>();
template <>
const char *Engine::read<const char*>();
template <>
std::string Engine::read<std::string>();

#endif
