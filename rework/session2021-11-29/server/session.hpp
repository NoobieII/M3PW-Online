//Lithio
//October 20, 2021

//session header file on the server side, mostly copied from 2019

#ifndef SESSION_HPP
#define SESSION_HPP

#include "aes/aes.h"

#include <string>
#include <boost/asio.hpp>
// PLEASE NOTE: Boost's shared_ptr is only used for Session. Use the C++ standard
// shared_ptr everywhere else.
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class Packet;
class PWServer;

class Session: public boost::enable_shared_from_this<Session>{
public:
	Session(PWServer *server);
	~Session();
	
	int get_session_id();
	
	//connect to address and port, and start async read for packet header
	//returns 0 upon success, -1 when failed
	void initialize();
	
	//disconnect
	void disconnect();
	
	//these three functions are called in the process of reading a packet,
	//and call each other in a cycle.
	void start_read_header();
	void read_header_handler(const boost::system::error_code &ec, size_t bytes_transferred);
	void read_body_handler(const boost::system::error_code &ec, size_t bytes_transferred);
	
	void send_packet(Packet *packet);
	void write_handler(const boost::system::error_code &ec, size_t bytes_transferred, char *send_buffer);
	boost::asio::ip::tcp::socket &get_socket();
	
	//packet handlers
	//client and server use
	void handle_test_message();
	//server use only
	void handle_send_ivs();
	
	//generic template for reading integer types
	//in the receive buffer
	template <typename T>
	T read(){
		T val;
		
		val = *(T*)(recv_ + read_length_);
		read_length_ += sizeof(T);
		
		return val;
	}
	
	void skip_bytes(size_t n);
private:
	size_t read_length_;
	boost::asio::ip::tcp::socket socket_;
	unsigned char *iv_send_;
	unsigned char *iv_recv_;
	AES_ctx send_ctx_;
	AES_ctx recv_ctx_;
	char *recv_;
	int session_id_;
	PWServer *server_;
};

template <>
bool Session::read<bool>();
template <>
const char *Session::read<const char*>();
template <>
std::string Session::read<std::string>();

#endif
