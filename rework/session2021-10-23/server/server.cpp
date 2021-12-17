//Lithio
//October 17, 2021

//server implementation file

#include "server.hpp"
#include "session.hpp"

#if (defined(unix) || defined(__unix) || defined(__unix__))
#include "unistd.h"
#endif

#if defined(_WIN32)
#include "windows.h"
#endif

PWServer::PWServer():
//timing the polling
deltatime_(0),
framerate_(0),
ticks_(0),
last_update_time_(0),
last_sleep_time_(0),
next_sleep_time_(0),
sessions_(),
is_accepting_connections_(false),
io_(),
session_id_(0)
{
}

PWServer::~PWServer(){
}

int PWServer::initialize(int framerate){
	if(framerate < 1 || framerate > 60){
		return -1;
	}
	
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
	
	is_accepting_connections_ = true;
	
	return 0;
}
	

//sleeps until the next polling
void PWServer::sleep_until_next_frame(){
	//if the current time is less than the time for next frame
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

//Checks all connected sessions for any received bytes
//returns 0 if successful and -1 if failed
int PWServer::poll_all_sessions(){
	io_.poll();
}

bool PWServer::is_accepting_connections(){
	return is_accepting_connections_;
}

int PWServer::get_session_id(){
	return session_id_++;
}

void PWServer::add_session(Session *session){
	sessions_[session->get_session_id()] = session->shared_from_this();
}

void PWServer::remove_session(int session_id){
	sessions_.erase(session_id);
}

boost::asio::io_service &PWServer::get_io_service(){
	return io_;
}

//Make a clean shutdown of all connections
//returns 0 if successful and -1 if failed
int PWServer::shutdown(){
	
	//disconnect all connections
	for(auto &it : sessions_){
		if(it.second->get_socket().is_open()){
			it.second->disconnect();
		}
	}
	sessions_.clear();
	
	return 0;
}
