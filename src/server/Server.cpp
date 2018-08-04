#include "Server.h"

#include <thread>
#include <iostream>
#include <functional>

#define MAXTHREADS 20

namespace HttpServer
{
   int Server::run()
   {					
        std::cout << "Server Initializing..." << std::endl;
        /*load config*/
        /* listen sockets */
        if(!InitListenPorts())
        {
           return 0x20;
        }

	/* Init Epoll & add listenfd */
	Socket::EpollProxy ep_proxy;
	if( !ep_proxy.create(MAXEVENT)) return EXIT_FAILURE ;
	for(Socket::Socket listen_sock : listeners)
	{
	   ep_proxy.addSocket(listen_sock);
	}
	
	/*Set Process*/
	this->controls.setProcess();
	std::cout <<"Server Started work..."<<std::endl;

	/*Listening*/
	SocketsQueue sockets;

	std::function<int (Server*, SocketsQueue &)> serverCycleQueue = std::mem_fn(&Server::cycleQueue);
	std::thread threadQueue(serverCycleQueue, this, std::ref(sockets));
	std::vector <Socket::Socket> accept_sockets;
	
	/*Create thread pool*/
	/*Timer*/
	/*Epoll wait loop*/

	do{
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

	       accept_sockets.clear();
	    }
  	 }
	 while(this->controls.process_flag);

	/*Clean*/
	threadQueue.join();
	ep_proxy.destroy();
	if(this->listeners.empty() == false)
	{
	   for(Socket::Socket &sock : this->listeners)
	   {
	   	sock.close();
	   }
	   this->listeners.clear();
	}

	this->clear();

	std::cout << "Server work completed..." <<std::endl;

	return EXIT_SUCCESS;
   }

   int Server::cycleQueue(SocketsQueue & sockets)
   {	
			//Thread count settup
			size_t threads_max_count =0;
			
			threads_max_count = std::thread::hardware_concurrency();
			if(0 == threads_max_count)
			{
				threads_max_count = 1;
			}
				threads_max_count *= 2;
			
			if(threads_max_count > MAXTHREADS) 
			{
				threads_max_count = MAXTHREADS;
			}
			std::cout<< "threads_max_count = " << threads_max_count <<std::endl;
			
			//Active thread count
			this->threads_working_count =0;
			
			std::vector <std::thread> active_threads;
			active_threads.reserve(threads_max_count);

			do{
				while(
						this->threads_working_count == active_threads.size() &&
						active_threads.size() < threads_max_count && 
						sockets.empty() == false)
				{

				}

				size_t notify_count = active_threads.size() - this->threads_working_count;
				if(notify_count > sockets.size())
				{
					notify_count = sockets.size();
				}
			}
			while(this->controls.process_flag);

			return 0;      
   }

   static void close_listeners(std::vector<Socket::Socket> &listeners) 
   {
      for (auto &sock : listeners)
      {
         sock.close();
      }
   }

   void Server::stop()
   {
      this->controls.stopProcess();
      close_listeners(this->listeners);
   }

   void Server::clear()
   {
      this->controls.clear();
   }
 
   bool Server::InitListenPorts()
   {
     std::cout << "Binding listening socket..." << std::endl;
     Socket::Socket listen_sock;
     listeners.emplace_back(listen_sock);

     for(Socket::Socket listen_sock : listeners)
     {
        if(!listen_sock.tryBindPort(PORT))
	{
   	   std::cout << "Binding Port failed..."<< std::endl;
	}
     }

     if(this->listeners.empty())
     {
        std::cout <<"Error any Socket was not open" <<std::endl;
	this->clear();
	return 0x20;
     }
     return true;
   }
}
