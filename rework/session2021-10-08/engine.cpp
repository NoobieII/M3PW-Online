//Lithio
//October 08, 2021
//
//Engine implementation file

#include <cstdio>
#include <cstring>
#include <ctime>

//windows specific
#include <winsock2.h>
//required definition for MINGW g++ compiler
#define _WIN32_WINNT 0x0501
#include <ws2tcpip.h>
//end of windows specific

#include "engine.hpp"
#include "packet.hpp"


Engine::Engine():
deltatime_(0),
framerate_(0),
ticks_(0),
last_update_time_(0),
last_sleep_time_(0),
next_sleep_time_(0),
sock_enabled_(0),
send_size_(0),
send_length_(0),
send_(nullptr),
recv_size_(0),
recv_length_(0),
read_length_(0),
recv_(nullptr)
//windows specific
,socket_(INVALID_SOCKET)
{
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
	
int Engine::connect(const char *host, short port){
	
	//windows implementation... check
	//https://docs.microsoft.com/en-us/windows/win32/winsock/initializing-winsock
	WSADATA wsadata;
	int num;
	
	addrinfo *result = NULL, *ptr = NULL, hints;
	char port_str[20];
	
	num = WSAStartup(MAKEWORD(2,2), &wsadata);
	if(num != 0){
		printf("WSAStartup failed: %d\n", num);
		return -1;
	}
	
	ZeroMemory(&hints, sizeof(hints));
	//use either AF_INET6, AF_INET or AF_UNSPEC
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	
	sprintf(port_str, "%d", port);
	
	//get address
	num = getaddrinfo(host, port_str, &hints, &result);
	if(num != 0){
		printf("getaddrinfo failed: %d\n", num);
		WSACleanup();
		return -1;
	}
	
	//attempt connection to each addrinfo structure until success
	ptr = result;
	socket_ = INVALID_SOCKET;
	while(socket_ == INVALID_SOCKET && ptr != NULL){
	
		socket_ = 
			socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		
		if(socket_ == INVALID_SOCKET){
			printf("Error at socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(ptr);
			WSACleanup();
			return -1;
		}
		
		num = ::connect(socket_, ptr->ai_addr, (int)ptr->ai_addrlen);
		if(num == SOCKET_ERROR){
			closesocket(socket_);
			socket_ = INVALID_SOCKET;
		}
		
		//check the next address that getaddrinfo returned
		ptr = ptr->ai_next;
	}
	
	//address no longer needed
	freeaddrinfo(result);
	result = NULL;
	
	if(socket_ == INVALID_SOCKET){
		printf("Unable to connect to the server!\n");
		WSACleanup();
		return -1;
	}
	
	//end of windows specific socket setup
	
	//set the buffers
	send_size_ = DEFAULT_BUFLEN;
	send_ = new char[send_size_];
	
	recv_size_ = DEFAULT_BUFLEN;
	recv_ = new char[recv_size_];
	
	//good to go
	sock_enabled_ = 1;
	return 0;
}

int Engine::send_packet(Packet *p){
	if(!sock_enabled_){
		return -1;
	}
	
	//TODO: resize send buffer
	
	//copy length of the packet body
	*(short*)(send_) = p->get_packet_length() - 2;
	
	//copy entire packet
	memcpy(send_ + 2, p->get_buffer(), p->get_packet_length());
	
	
	//windows specific code
	int num;
	
	if(sock_enabled_){
		num = send(socket_, send_, p->get_packet_length() + 2, 0);
		
		if(num == SOCKET_ERROR){
			printf("send failed: %d\n", WSAGetLastError());
		}
		printf("sent %d bytes\n", num);
	}
	//end of windows specific code
	
	return 0;
}

int Engine::poll_recv_packets(){
	int body_length;
	
	//windows specific code
	int num;
	
	fd_set set;
	timeval tv;
	
	//check for received bytes
	if(sock_enabled_){
		//check if the socket is readable and copy all received bytes
		//to the receive buffer recv_
		set.fd_count = 1;
		set.fd_array[0] = socket_;
		
		//time interval of .1 second
		//tv.tv_sec = 0;
		//tv.tv_usec = 100000;
		
		//check if socket will not block, max wait time of .1 second
		//num = select(0, &set, NULL, NULL, &tv);
		
		num = select(0, &set, NULL, NULL, NULL);
		
		//one socket is readable
		while(num == 1){
			//attempt peek read 4 bytes - if unable to do so, try again next
			//time
			num = recv(socket_, recv_, 4, MSG_PEEK);
			
			if(num == 0){
				printf("Socket is disconnected.\n");
			}
			
			if(num != 4){
				return 0;
			}
			
			//attempt read the rest of the body(if exists) - if unable to do so,
			//try again next time
			body_length = *(short*)(recv_);
			
			if(body_length != 0){
				//read the socket - note that it rereads the first 4 bytes
				//TODO: resize recv_ if necessary
				num = recv(socket_, recv_, body_length + 4, MSG_PEEK);
			}
			
			if(num != body_length + 4){
				return 0;
			}
			
			//the socket's buffer can be discarded now
			//TODO: find a function (if possible) that does this without reread
			recv(socket_, recv_, body_length + 4, 0);
			
			//indicate that there is data to be read
			recv_length_ + body_length + 4;
			//printf("received %d bytes.\n", num);
			
			//check if there is more readability
			num = select(0, &set, NULL, NULL, NULL);
		}
		
	}
	//end of windows specific code
	
	return 0;
}

void Engine::sleep_until_next_frame(){
	//if the current time is less than the time for next frame
	int current_time = clock() / (CLOCKS_PER_SEC / 1000);
	
	if(current_time < next_sleep_time_){
		//windows specific
		Sleep(next_sleep_time_ - current_time);
	}
	
	last_sleep_time_ = next_sleep_time_;
	next_sleep_time_ = last_sleep_time_ + (1000 / framerate_);
	
	ticks_++;
}

void Engine::poll_all_events(){
	if(sock_enabled_){
		poll_recv_packets();
	}
	
	//todo when key strokes are to be handled...
}

void Engine::update_all(){
	int num;
	int packet_body_length;
	const char *msg;
	
	//handle received packets (if sockets enabled)
	if(sock_enabled_){
		
		//TODO sequentially review each packet
		if(recv_length_ > 0){
			printf("received %d bytes\n", recv_length_);
			recv_length_ = 0;
		}
	}
	
	//write something to send buffer every second
	if(ticks_ % framerate_ == 0){
		Packet packet;
		
		std::string str = "hello... here's a number: " + ticks_;
		packet.test_message(str.c_str());
		send_packet(&packet);
	}
}

Engine::~Engine(){
	if(send_){
		delete[] send_;
	}
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

