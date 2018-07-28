#include <iostream>
#include <csignal>

#include "socket/Socket.h"
#include "utils/Utils.h"
#include "system/System.h"

int main (const int argc, char ** argv)
{
	std::cout << "Server Initializing..." << std::endl;
	
	/*
	 *load config
	 * */
	
	std::cout << "Loading Config..." << std::endl;

	std::cout << "Config Loaded...\n" <<std::endl;
	/*
	 *signal handler for SIGPIPE
	 * */
	std::cout << "Setting signal handler..." << std::endl;
	struct ::sigaction sa;	
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	if(::sigaction(SIGPIPE, &sa, nullptr))
	{
		std::cout << "Install signal handler for SIGPIPE failed"<<std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Signal handler for SIGPIPE done...\n" << std::endl;

	/*
	 *bind sockets
	 * */
	std::cout << "Binding socket..." << std::endl;
	//native_socket_type listenfd = Socket::TryBindSocket();
	std::cout << "Socket ready...\n" << std::endl;

	/*
	 *Init Epoll & add listenfd
	 * */
	std::cout << "Initializing Epoll..." << std::endl;
	
	std::cout << "Epoll initialized ...\n" << std::endl;

	/*
	 *Create thread pool
	 * */
	std::cout << "Creating thread pool..." << std::endl;
	std::cout << "Threadpool Initialzied...\n" << std::endl;
	/*
	 * Timer
	 * */
	std::cout << "Initializing Timer..." << std::endl;
	std::cout << "Timer Initialized...\n" << std::endl;

	/*
	 *Epoll wait loop
	 * */
	std::cout << "Epoll_wait loop..." << std::endl;

	while(1)
	{
		break;
	}
	
	return 0;
}
