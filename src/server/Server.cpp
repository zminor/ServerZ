#include "Server.h"

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
