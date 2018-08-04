#include "Server.h"
#include "../utils/Event.h"
#include "../socket/AdapterDefault.h"
#include "protocol/ServerProtocol.h"
#include "protocol/ServerHttp2Stream.h"

#include <thread>
#include <iostream>
#include <functional>

#define MAXTHREADS 20

namespace HttpServer
{

	static std::unique_ptr<ServerProtocol> getProtocolVariant(
									Socket::Adapter &sock,
									const ServerSettings &settings,
									ServerControls &controls,
									SocketsQueue &sockets,
									Http2::IncStream *stream)					
	{
		std::unique_ptr<ServerProtocol> prot;
		if (stream)
		{
			prot.reset(new ServerHttp2Stream(sock, settings, controls, stream) );
			return prot;
		}

		return prot;
	}

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
			
			Utils::Event eventThreadCycle(false, true);

			std::function < void (Server *, SocketsQueue&, Utils::Event &)> serverThreadRequestCycle = std::mem_fn(&Server::threadRequestCycle);

			std::vector <std::thread> active_threads;
			active_threads.reserve(threads_max_count);

			do{

				while(
						this->threads_working_count == active_threads.size() &&
						active_threads.size() < threads_max_count && 
						sockets.empty() == false
						)
						{
							active_threads.emplace_back(
										serverThreadRequestCycle,
										this,
										std::ref(sockets),
										std::ref(eventThreadCycle)
							);
						}

				size_t notify_count = active_threads.size() - this->threads_working_count;

				if(notify_count > sockets.size())
				{
					notify_count = sockets.size();
				}

				eventThreadCycle.notify(notify_count);
				this->controls.eventProcessQueue -> wait();
			}
			while(this->controls.process_flag);
			
			eventThreadCycle.notify();
			if(active_threads.empty() == false)
			{
				for(auto &th: active_threads)
				{
					th.join();
				}
				active_threads.clear();
			}
		//	this->controls.eventNotFullQueue->notify();
			return 0;      
   }

	 void Server::threadRequestCycle(
				SocketsQueue &sockets,
				Utils::Event &eventThreadCycle
	 ) const
	 {
	 		while(true)
			{
				Socket::Socket sock;
				Http2::IncStream * stream = nullptr;

				eventThreadCycle.wait();

				if(this->controls.process_flag == false)
				{
					break;
				}

				sockets.lock();

				if(sockets.size())
				{
					std::tie(sock,stream) = sockets.front();
					sockets.pop();
				}

				if(sockets.empty())
				{
					eventThreadCycle.reset();
				//	this->controls.eventNotFullQueue->notify();
				}
				sockets.unlock();
				//not empty
				if(sock.is_open())
				{
					++this->threads_working_count;
					::sockaddr_in sock_addr{};
					::socklen_t sock_addr_len = sizeof(sock_addr);

					::getsockname(
									sock.get_handle(),
									reinterpret_cast <sockaddr*> (&sock_addr),
									&sock_addr_len
						);

					//const int port = ntohs(sock_addr.sin_port);

					Socket::AdapterDefault socket_adapter(sock);
					this->threadRequestProc(
													socket_adapter,
													sockets,
													stream
													);
					--this->threads_working_count;
				}
			}
	 }
	void Server::threadRequestProc(
									Socket::Adapter &sock,
									SocketsQueue &sockets,
									Http2::IncStream *stream
									)const
	{
					
		std::unique_ptr <ServerProtocol> prot = getProtocolVariant(
												sock,
												this->settings,
												this->controls,
												sockets,
												stream										
										);	
	if (prot) {
				// Check if switching protocol
			/*	for (ServerProtocol *ret = nullptr; ; )
				{
								ret = prot->process();
								
								if (prot.get() == ret) {
												break;
								}
								prot.reset(ret);
								
				}
				*/
				prot->close();
				 																																}
	}

//-----------------------------------------------------------------------------------------//
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

		 this->tryBindPort(PORT);

     if(this->listeners.empty())
     {
        std::cout <<"Error any Socket was not open" <<std::endl;
				this->clear();
				return 0x20;
     }
     return true;
   }

	 	bool Server::tryBindPort(
										const int port
										)
		{
			if (ports.cend() != ports.find(port) )
			{
				return false;
			}
			
			Socket::Socket sock;
			
			if (sock.open() == false)
			{
				std::cout << "Error: socket cannot be open; errno " << Socket::Socket::getLastError() << ";" << std::endl;
				return false;
			}

			if (sock.bind(port) == false)
			{
				std::cout << "Error: port " << port << " cannot be bind; errno " << Socket::Socket::getLastError() << ";" << std::endl;
				return false;
			}

			if (sock.listen() == false)
			{
				std::cout << "Error: socket " << port << " cannot be listen; errno " << Socket::Socket::getLastError() << ";" << std::endl;
				return false;
			}

			sock.nonblock(true);

			this->listeners.emplace_back(std::move(sock) );

			this->ports.emplace(port);

			return true;
		}
}

