#include <iostream>
#include <csignal>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>

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
	System :: native_socket_type listenfd ;
	struct sockaddr_in client_addr;
	socklen_t inlen =1;
	::memset(&client_addr, 0, sizeof(sockaddr_in));

	//listenfd = Socket::bindPort(PORT);

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
