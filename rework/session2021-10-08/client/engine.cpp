//Lithio
//October 08, 2021
//
//Engine implementation file

#include <cstdio>
#include <cstring>
#include <ctime>

#include <string>

#include <boost/bind.hpp>

#include "constants.hpp"
#include "engine.hpp"
#include "packet.hpp"
#include "random/random.hpp"


Engine::Engine():
deltatime_(0),
framerate_(0),
ticks_(0),
last_update_time_(0),
last_sleep_time_(0),
next_sleep_time_(0),
socket_enabled_(false),
read_length_(0),
io_(),
socket_(io_),
iv_send_(new unsigned char[16]),
iv_recv_(new unsigned char[16]),
send_ctx_(),
recv_ctx_(),
recv_(nullptr)
{
	memset(iv_send_, 0, 16);
	memset(iv_recv_, 0, 16);
}

int Engine::initialize(
	int width,
	int height,
	int framerate,
	const char *caption){
	
	framerate_ = framerate;
	last_update_time_ = clock() / (CLOCKS_PER_SEC / 1000);
	last_sleep_time_ = last_update_time_;
	next_sleep_time_ = last_update_time_ + (1000 / framerate_);
	
	return 0;
}
	
int Engine::connect(std::string address, unsigned short port){
	int ret = 0;
	int i;
	
	try{
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::from_string(address), port);
		boost::asio::ip::tcp::resolver resolver(socket_.get_io_service());
		boost::asio::connect(socket_, resolver.resolve(endpoint));
	}
	catch(...){
		ret = -1;
	}
	
	if(ret == -1){
		return ret;
	}
	
	//initialize with zero'd iv's
	AES_init_ctx_iv(&send_ctx_, AES128_KEY, iv_send_);
	AES_init_ctx_iv(&recv_ctx_, AES128_KEY, iv_recv_);
	
	//CLIENT ONLY vv
	//generate iv's and send to server, then initialize context with iv's
	for(i = 0; i < 16; ++i){
		iv_send_[i] = random::rand_int() % 256;
		iv_recv_[i] = random::rand_int() % 256;
	}
	
	//send the server the iv's before setting the AES with the iv's
	Packet packet;
	packet.send_ivs(iv_send_, iv_recv_);
	send_packet(&packet);
	
	//set iv's
	AES_init_ctx_iv(&send_ctx_, AES128_KEY, iv_send_);
	AES_init_ctx_iv(&recv_ctx_, AES128_KEY, iv_recv_);
	//CLIENT ONLY ^^
	
	//start async read
	start_read_header();
	
	socket_enabled_ = true;
	return 0;
}

//disconnect
void Engine::disconnect(){
	socket_.close();
}

//these three functions are called in the process of reading a packet,
//and call each other in a cycle.
void Engine::start_read_header(){
	recv_ = new char[4];
	boost::asio::async_read(
		socket_,
		boost::asio::buffer(recv_, 4),
		boost::bind(
			&Engine::read_header_handler,
			/*shared_from_this()*/this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void Engine::read_header_handler(const boost::system::error_code &ec, size_t bytes_transferred){
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
				&Engine::read_body_handler,
				/*shared_from_this()*/this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}
	else{
		read_body_handler(ec, bytes_transferred);
	}
}

void Engine::read_body_handler(const boost::system::error_code &ec, size_t bytes_transferred){
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
	case 32766:
		handle_test_message();
		break;
	}
	
	printf("received %d bytes: ", (int)body_length);
	fwrite(recv_, body_length + 4, 1, stdout);
	printf("\n");
	
	delete[] recv_;
	recv_ = nullptr;
	
	read_length_ = 0;
	start_read_header();
}

void Engine::send_packet(Packet *packet){
	char *send_buffer;
	short packet_body_length;
	
	packet_body_length = packet->get_packet_length() - 2;
	send_buffer = new char[packet_body_length + 4];
	
	*(short*)(send_buffer) = packet_body_length;
	memcpy(send_buffer + 2, packet->get_buffer(), packet->get_packet_length());
	
	printf("sending packet length %d, header %d, body: ", *(short*)(send_buffer), *(short*)(send_buffer + 2));
	fwrite(send_buffer + 4,  *(short*)(send_buffer), 1, stdout);
	printf("\n");
	
	//encrypt
	AES_CTR_xcrypt_buffer(&send_ctx_, (unsigned char*)(send_buffer + 4), packet_body_length);
	
	//send the the length of the packet and the packet itself
	boost::asio::async_write(
		socket_,
		boost::asio::buffer(send_buffer, packet_body_length + 4),
		boost::bind(
			&Engine::write_handler,
			/*shared_from_this()*/this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred,
			send_buffer
		)
	);
}

void Engine::write_handler(const boost::system::error_code &ec, size_t bytes_transferred, char *send_buffer){
	if(ec || bytes_transferred < 4){
		disconnect();
	}
	delete[] send_buffer;
}

void Engine::handle_test_message(){
	printf("Test message received: %s\n", recv_ + 6);
}

void Engine::sleep_until_next_frame(){
	//if the current time is less than the time for next frame
	int current_time = clock() / (CLOCKS_PER_SEC / 1000);
	
	if(current_time < next_sleep_time_){
		//windows specific code
		Sleep(next_sleep_time_ - current_time);
		//end of windows specific code
	}
	
	last_sleep_time_ = next_sleep_time_;
	next_sleep_time_ = last_sleep_time_ + (1000 / framerate_);
	
	ticks_++;
}

void Engine::poll_all_events(){
	//todo when key strokes are to be handled...
	
	//check socket and handle any incoming packets
	if(socket_enabled_){
		io_.poll();
	}
}

void Engine::update_all(){
	int num;
	int packet_body_length;
	
	//write something to send buffer every second
	if(ticks_ % framerate_ == 0){
		Packet packet;
		
		std::string str = "hello... here's a number: ";
		str += ticks_;
		packet.test_message(str.c_str());
		send_packet(&packet);
	}
}

Engine::~Engine(){
	delete[] iv_send_;
	delete[] iv_recv_;
	if(recv_){
		delete[] recv_;
	}
}

//template specialization for c string type - be careful using this
template <>
const char *Engine::read<const char*>(){
	short string_length;
	const char *val;
	
	string_length = read<short>();
	val = recv_ + read_length_;
	read_length_ += string_length;
	
	return val;
}

//template specialization for string type
template <>
std::string Engine::read<std::string>(){
	std::string val;
	val = read<const char*>();
	return val;
}

