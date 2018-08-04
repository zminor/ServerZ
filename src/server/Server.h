#ifndef __SERVER_H__
#define __SERVER_H__

#include <iostream>
#include <csignal>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdint.h>
#include <vector>
#include <unordered_set>

#include "../utils/Event.h"
#include "../utils/Utils.h"
#include "../system/System.h"
#include "../socket/Socket.h"
#include "../socket/EpollProxy.h"
#include "../server/SocketsQueue.h"
#include "../transfer/http2/Http2.h"
#include "ServerControls.h"
#include "SocketsQueue.h"
#include "../socket/AdapterDefault.h"

const int PORT =  3000;
#define MAXEVENT 1024

namespace HttpServer
{

	class Server
	{
	public:
		mutable ServerControls controls;

	protected:
		std::vector<Socket::Socket> listeners;
		mutable std::atomic_size_t threads_working_count;
		std::unordered_set <int> ports;
	protected:
		bool init();
		void clear();
		bool  InitListenPorts();

		int cycleQueue(SocketsQueue &sockets);
		
	 	void threadRequestCycle(
				SocketsQueue &sockets,
				Utils::Event &eventThreadCycle
	 	) const ;
		
		void threadRequestProc(
										Socket::Adapter &sock,
										SocketsQueue &sockets,
										Http2::IncStream *stream
										) const;
	public:
		Server()=default;
		int run();
		void stop();

		bool tryBindPort(const int port	);
	};
}

#endif
