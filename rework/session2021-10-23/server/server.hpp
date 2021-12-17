//Lithio
//October 08, 2021
//
//Server header file

#ifndef PWSERVER_HPP
#define PWSERVER_HPP

#include <map>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

class Session;

class PWServer{
public:
	//default constructor
	PWServer();
	
	//destructor
	~PWServer();
	
	//bind an acceptor, and sets the poll rate - should be the
	//same as the client's frame rate
	//returns 0 if successful and -1 if failed
	int initialize(int framerate);
	
	//sleeps until the next polling
	void sleep_until_next_frame();
	
	//Checks all connected sessions for any received bytes
	//returns 0 if successful and -1 if failed
	int poll_all_sessions();
	
	bool is_accepting_connections();
	
	//gives a unique session id
	int get_session_id();
	
	//add/remove from sessions_ container
	void add_session(Session *session);
	void remove_session(int session_id);
	
	boost::asio::io_service &get_io_service();
	
	//Make a clean shutdown of all connections
	//returns 0 if successful and -1 if failed
	int shutdown();
private:
	//timing the polling
	int deltatime_;
	int framerate_;
	int ticks_;
	int last_update_time_;
	int last_sleep_time_;
	int next_sleep_time_;
	
	bool is_accepting_connections_;
	boost::asio::io_service io_;
	int session_id_;
	std::map<int, boost::shared_ptr<Session> > sessions_;
};


#endif