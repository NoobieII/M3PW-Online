//Lithio
//October 08, 2021
//
//Engine implementation file

#include <cstdio>
#include <cstring>
#include <ctime>

#if (defined(unix) || defined(__unix) || defined(__unix__))
#include "unistd.h"
#endif

#if defined(_WIN32)
#include "windows.h"
#endif

#include <string>

#include <boost/bind.hpp>

#include "constants.hpp"
#include "packet.hpp"
#include "pwengine.hpp"
#include "random/random.hpp"



PWEngine::PWEngine():
deltatime_(0),
framerate_(0),
ticks_(0),
last_update_time_(0),
last_sleep_time_(0),
next_sleep_time_(0),
is_key_lifted_(),
is_key_pressed_(),
is_focused_(false),
text_input_(),
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

int PWEngine::initialize(
	int width,
	int height,
	int framerate,
	const char *caption){
	
	//initialize time
	framerate_ = framerate;
	
	#if (defined(unix) || defined(__unix) || defined(__unix__))
	struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC, &tp);
	
	last_update_time_ = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
	#endif
	
	#if defined(_WIN32)
	last_update_time_ = clock() / (CLOCKS_PER_SEC / 1000);
	#endif
	
	last_sleep_time_ = last_update_time_;
	next_sleep_time_ = last_update_time_ + (1000 / framerate_);
	
	//initialize input handling
	#if defined(_WIN32)
	input_handle_ = GetStdHandle(STD_INPUT_HANDLE);
	if(input_handle_ == INVALID_HANDLE_VALUE){
		printf("PWEngine::initialize() could not get input handle.\n");
		return -1;
	}
	#endif
	
	return 0;
}
	
int PWEngine::connect(std::string address, unsigned short port){
	int ret = 0;
	int i;
	
	try{
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::from_string(address), port);
		boost::asio::ip::tcp::resolver resolver(io_);
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
		iv_send_[i] = pw_rng::rand_int() % 256;
		iv_recv_[i] = pw_rng::rand_int() % 256;
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
void PWEngine::disconnect(){
	socket_.close();
}

//these three functions are called in the process of reading a packet,
//and call each other in a cycle.
void PWEngine::start_read_header(){
	recv_ = new char[4];
	boost::asio::async_read(
		socket_,
		boost::asio::buffer(recv_, 4),
		boost::bind(
			&PWEngine::read_header_handler,
			/*shared_from_this()*/this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void PWEngine::read_header_handler(const boost::system::error_code &ec, size_t bytes_transferred){
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
				&PWEngine::read_body_handler,
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

void PWEngine::read_body_handler(const boost::system::error_code &ec, size_t bytes_transferred){
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

void PWEngine::send_packet(Packet *packet){
	char *send_buffer;
	short packet_body_length;
	
	if(!socket_enabled_){
		return;
	}
	
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
	//boost::asio::async_write(
	//	socket_,
	//	boost::asio::buffer(send_buffer, packet_body_length + 4),
	//	boost::bind(
	//		&PWEngine::write_handler,
	//		/*shared_from_this()*/this,
	//		boost::asio::placeholders::error,
	//		boost::asio::placeholders::bytes_transferred,
	//		send_buffer
	//	)
	//);
	boost::asio::write(
		socket_,
		boost::asio::buffer(send_buffer, packet_body_length + 4)
	);
}

void PWEngine::write_handler(const boost::system::error_code &ec, size_t bytes_transferred, char *send_buffer){
	if(ec || bytes_transferred < 4){
		disconnect();
	}
	delete[] send_buffer;
}

void PWEngine::handle_test_message(){
	printf("Test message received: %s\n", recv_ + 6);
}

void PWEngine::sleep_until_next_frame(){
	//Put the thread to sleep if the current time is less than the time for
	//next frame
	int current_time;
	
	#if (defined(unix) || defined(__unix) || defined(__unix__))
	struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC, &tp);
	
	current_time = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
	#endif
	
	#if defined(_WIN32)
	current_time = clock() / (CLOCKS_PER_SEC / 1000);
	#endif
	
	if(current_time < next_sleep_time_){
		//linux specific
		#if (defined(unix) || defined(__unix) || defined(__unix__))
		usleep((next_sleep_time_ - current_time) * 1000);
		#endif

		//windows specific
		#if defined(_WIN32)
		Sleep(next_sleep_time_ - current_time);
		#endif
	}
	
	last_sleep_time_ = next_sleep_time_;
	next_sleep_time_ = last_sleep_time_ + (1000 / framerate_);
	
	ticks_++;
}

void PWEngine::poll_all_events(){
	//get key strokes
	int keycode;
	char text_char[4];
	
	#if defined(_WIN32)
	INPUT_RECORD events[100];
	DWORD num_events;
	int i;
	
	//read (without blocking) the console input buffer
	PeekConsoleInput(input_handle_, events, 100, &num_events);
	FlushConsoleInputBuffer(input_handle_);
	
	//register the events
	for(i = 0; i < num_events; ++i){
		switch(events[i].EventType){
		case FOCUS_EVENT:
			//microsoft says to ignore FOCUS_EVENT, so I am unsure on
			//how reliable this event type is.
			if(events[i].Event.FocusEvent.bSetFocus == TRUE){
				printf("Event.FocusEvent.bSetFocus is TRUE.\n");
				is_focused_ = true;
			}
			else{
				printf("Event.FocusEvent.bSetFocus is FALSE.\n");
				is_focused_ = false;
			}
			break;
		case KEY_EVENT:
			keycode = (int)(events[i].Event.KeyEvent.wVirtualKeyCode);
			keystate = events[i].Event.KeyEvent.dwControlKeyState;
			
			if(events[i].Event.KeyEvent.bKeyDown == TRUE){
				printf("Event.KeyEvent.bKeyDown is TRUE.\n");
				is_key_pressed_[keycode] = 1;
				
				text_char[0] = events[i].Event.KeyEvent.uChar.AsciiChar;
				
				//interpret text input
				if(text_char[0] != 0){
					//concatenate the engine's stored string
					text_char[1] = '\0';
					strncat(text_input_, text_char, 32);
				}
			}
			else{
				printf("Event.KeyEvent.bKeyDown is FALSE.\n");
				is_key_lifted_[keycode] = 1;
			}
			
			printf("Event.KeyEvent.wVirtualKeyCode = %x\n", keycode);
			//see https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
			
			break;
		case MENU_EVENT:
			//microsoft says to ignore these
			printf("menu event\n");
			break;
		case MOUSE_EVENT:
			printf("mouse event\n");
			break;
		case WINDOW_BUFFER_SIZE_EVENT:
			printf("window buffer size event\n");
			break;
		default:
			printf("unknown event type\n");
			break;
		}
	}
	#endif //#if defined(_WIN32)
	
	//check socket and handle any incoming packets
	if(socket_enabled_){
		while(io_.poll() > 1);
	}
}

void PWEngine::update_all(){
	int num;
	int packet_body_length;
	
	deltatime_ = next_sleep_time_ - last_sleep_time_;
	last_update_time_ = last_sleep_time_;
	
	//write something to send buffer every second
	if(ticks_ % framerate_ == 0){
		Packet packet;
		
		char str[40];
		sprintf(str, "hello... here's a number: %d", ticks_);
		packet.test_message(str);
		send_packet(&packet);
	}
	
	//clear all events
	memset(is_key_pressed_, 0, 256);
	memset(is_key_lifted_, 0, 256);
	text_input_[0] = '\0';
}

//time difference in milliseconds since last update
int PWEngine::deltatime(){
	return deltatime_;
}

//number of game ticks since the start of the program
int PWEngine::ticks(){
	return ticks_;
}

bool PWEngine::is_key_pressed(int keycode){
	if(keycode < 0 || keycode > 255){
		return false;
	}
	
	return is_key_pressed_[keycode];
}

bool PWEngine::is_key_lifted(int keycode){
	if(keycode < 0 || keycode > 255){
		return false;
	}
	
	return is_key_lifted_[keycode];
}

const char *PWEngine::get_text_input(){
	return text_input_;
}

PWEngine::~PWEngine(){
	delete[] iv_send_;
	delete[] iv_recv_;
	if(recv_){
		delete[] recv_;
	}
}

//template specialization for c string type - be careful using this
template <>
const char *PWEngine::read<const char*>(){
	short string_length;
	const char *val;
	
	string_length = read<short>();
	val = recv_ + read_length_;
	read_length_ += string_length;
	
	return val;
}

//template specialization for string type
template <>
std::string PWEngine::read<std::string>(){
	std::string val;
	val = read<const char*>();
	return val;
}

