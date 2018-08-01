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


#include "../utils/Utils.h"
#include "../system/System.h"
#include "../socket/Socket.h"
#include "../socket/EpollProxy.h"
#include "../server/SocketsQueue.h"
#include "../transfer/http2/Http2.h"
#include "ServerControls.h"
#include "SocketsQueue.h"

#define PORT 3000
#define MAXEVENT 1024

namespace HttpServer
{

	class HttpServer
	{
	public:
		mutable ServerControls controls;

	protected:
		std::vector<Socket::Socket> liseners;
		
	protected:
		int run();
		bool init();
		void clear();

	public:
	HttpServer()=default;

	


	};
}

#endif
