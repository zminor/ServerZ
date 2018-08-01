#include "Server.h"

namespace HttpServer
{

	int HttpServer::run()
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
		Socket::Socket listen_sock;
		std::vector <Socket::Socket> listeners;
		listeners.emplace_back(listen_sock);

		for(Socket::Socket listen_sock : listeners)
		{
			if(!listen_sock.tryBindPort(PORT))
			{
			std::cout << "Binding Port failed..."<< std::endl;
			}
		}

		/*
		 *Init Epoll & add listenfd
		 * */
		std::cout << "Initializing Epoll..." << std::endl;
		Socket::EpollProxy ep_proxy;
		if( ep_proxy.create(MAXEVENT))
		{
			std::cout << "Epoll initialized ...\n" << std::endl;
		}else 
		{
			std::cout <<"Epoll failed... \n" <<std::endl;
		}

		for(Socket::Socket listen_sock : listeners)
		{
				ep_proxy.addSocket(listen_sock);
		}

		SocketsQueue sockets;
		std::vector <Socket::Socket> accept_sockets;
	
		if( ep_proxy.accept(accept_sockets) )
		{
			sockets.lock();

			for(const Socket::Socket & sock: accept_sockets)
			{
				if(sock.is_open())
				{
					sock.nonblock(true);
					sockets.emplace(
						std::tuple<Socket::Socket, Http2::IncStream*>	
						{
							sock,
							nullptr
						}
					);
				}
			}

			sockets.unlock();
		}
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
				
	}

}
