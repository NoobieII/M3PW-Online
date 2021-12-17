//Lithio
//October 20, 2021

//session implementation on the server side
//mostly code copied from 2019

#include "constants.hpp"
#include "packet.hpp"
#include "random/random.hpp"
#include "server.hpp"
#include "session.hpp"

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <cstdio>

using namespace boost::asio::ip;

Session::Session(PWServer *server):
	read_length_(0),
	socket_(server->get_io_service()),
	iv_send_(new unsigned char[16]),
	iv_recv_(new unsigned char[16]),
	send_ctx_(),
	recv_ctx_(),
	recv_(nullptr),
	session_id_(server->get_session_id()),
	server_(server)
{
	memset(iv_send_, 0, 16);
	memset(iv_recv_, 0, 16);
}

Session::~Session(){
	delete[] iv_send_;
	delete[] iv_recv_;
	if(recv_){
		delete[] recv_;
	}
}

int Session::get_session_id(){
	return session_id_;
}

void Session::initialize(){
	//initialize with zero'd iv's
	AES_init_ctx_iv(&send_ctx_, AES128_KEY, iv_send_);
	AES_init_ctx_iv(&recv_ctx_, AES128_KEY, iv_recv_);
	
	//start async read (expecting recv/send iv's from client)
	start_read_header();
}

void Session::disconnect(){
	socket_.close();
	
	//remove this from the server sessions_ container
	server_->remove_session(session_id_);
}

void Session::start_read_header(){
	recv_ = new char[4];
	boost::asio::async_read(
		socket_,
		boost::asio::buffer(recv_, 4),
		boost::bind(
			&Session::read_header_handler,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void Session::read_header_handler(const boost::system::error_code &ec, size_t bytes_transferred){
	short body_length;
	char buf[4];
	
	//normally the packet header (packet body size and header number) should be 4 bytes
	//close the session and stop the read cycle if that happens.
	if(ec || bytes_transferred < 4){
		disconnect();
		return;
	}
	
	body_length = *(short*)(recv_);
	if(body_length > 0){
		//resize the receive buffer
		memcpy(buf, recv_, 4);
		delete[] recv_;
		recv_ = new char[body_length + 4];
		memcpy(recv_, buf, 4);
		
		//read the rest of the packet (the body)
		boost::asio::async_read(
			socket_,
			boost::asio::buffer(recv_ + 4, body_length),
			boost::bind(
				&Session::read_body_handler,
				shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}
	else{
		read_body_handler(ec, bytes_transferred);
	}
}

void Session::read_body_handler(const boost::system::error_code &ec, size_t bytes_transferred){
	short body_length;
	short packet_header;
	
	if(ec){
		disconnect();
	}
	
	body_length = *(short*)(recv_);
	packet_header = *(short*)(recv_ + 2);
	
	//decrypt
	AES_CTR_xcrypt_buffer(&recv_ctx_, (unsigned char*)(recv_ + 4), body_length);
	
	//set read length to 4
	read_length_ = 4;
	
	//handle the packet
	switch(packet_header){
	case 32767:
		handle_send_ivs();
		break;
	case 32766:
		handle_test_message();
		break;
	};
	
	printf("received %d bytes: ", (int)body_length);
	fwrite(recv_, body_length + 4, 1, stdout);
	printf("\n");
	
	delete[] recv_;
	recv_ = nullptr;
	
	read_length_ = 0;
	start_read_header();
}

void Session::send_packet(Packet *packet){
	char *send_buffer;
	short packet_body_length;
	
	//create length, packet header and packet body
	packet_body_length = packet->get_packet_length() - 2;
	send_buffer = new char[packet_body_length + 4];
	
	*(short*)(send_buffer) = packet_body_length;
	memcpy(send_buffer + 2, packet->get_buffer(), packet->get_packet_length());
	
	//encrypt
	AES_CTR_xcrypt_buffer(&send_ctx_, (unsigned char*)(send_buffer + 4), packet_body_length);
	
	//send the the length of the packet and the packet itself
	boost::asio::async_write(
		socket_,
		boost::asio::buffer(send_buffer, packet_body_length + 4),
		boost::bind(
			&Session::write_handler,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred,
			send_buffer
		)
	);
}

void Session::write_handler(const boost::system::error_code &ec, size_t bytes_transferred, char *send_buffer){
	if(ec || bytes_transferred < 4){
		disconnect();
	}
	delete[] send_buffer;
}

boost::asio::ip::tcp::socket &Session::get_socket(){
	return socket_;
}

//template specialization for bool types
template <>
bool Session::read<bool>(){
	return read<char>();
}

//template specialization for c string type - be careful using this
template <>
const char *Session::read<const char*>(){
	short string_length;
	const char *val;
	
	string_length = read<short>();
	val = recv_ + read_length_;
	read_length_ += string_length;
	
	return val;
}

//template specialization for string type
template <>
std::string Session::read<std::string>(){
	std::string val;
	val = read<const char*>();
	return val;
}

void Session::skip_bytes(size_t n){
	read_length_ += n;
}

void Session::handle_send_ivs(){
	//Packet header number 32767 is for the client sending iv's.
	//Note the order of the receive/send iv's.
	memcpy(iv_recv_, recv_ + 4, 16);
	memcpy(iv_send_, recv_ + 20, 16);
	
	AES_ctx_set_iv(&recv_ctx_, iv_recv_);
	AES_ctx_set_iv(&send_ctx_, iv_send_);
	//AES_init_ctx_iv(&recv_ctx_, AES128_KEY, iv_recv_);
	//AES_init_ctx_iv(&send_ctx_, AES128_KEY, iv_send_);
}

void Session::handle_test_message(){
	printf("Test message received: %s\n", recv_ + 6);
}
