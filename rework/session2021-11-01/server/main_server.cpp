//Lithio
//October 16, 2021

//program for a small server application using the PWEngine
#include <cstdio>

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include "pwserver.hpp"
#include "random/random.hpp"
#include "session.hpp"

void server_acceptor_thread();

PWServer *server = nullptr;

int main(int argc, char *argv[]){
	server = new PWServer;
	
	//initialize random number generator
	pw_rng::init();
	
	//poll 3 times per second
	if(server->initialize(3) == -1){
		printf("could not initialize\n");
		return -1;
	}
	
	//run the acceptor thread (start accepting connections)
	boost::thread thread1(server_acceptor_thread);
	
	while(server->is_accepting_connections()){
		server->sleep_until_next_frame();
		
		//poll all client sessions and respond to incoming data
		server->poll_all_sessions();
		
	}
	
	server->shutdown();
	
	delete server;
	
	return 0;
}

void server_acceptor_thread(){
	boost::asio::ip::tcp::acceptor acceptor(server->get_io_service());
	boost::system::error_code ec;
	
	//open the acceptor
	acceptor.open(boost::asio::ip::tcp::v4());
	
	//set acceptor option - don't reuse addresses
	acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(false));
	
	//bind
	acceptor.bind(
		boost::asio::ip::tcp::endpoint(
			boost::asio::ip::tcp::v4(),
			16555
		)
	);
	
	//listen - make maximum 10 connections queued in backlog
	acceptor.listen(10);
	
	while(server->is_accepting_connections()){
		boost::shared_ptr<Session> new_connection(new Session(server));
		
		acceptor.accept(new_connection->get_socket(), ec);
		
		if(!ec){
			new_connection->initialize();
			server->add_session(new_connection.get());
		}
	}
}
