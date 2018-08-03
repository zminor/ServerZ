#include "server/Server.h"
#include "SignalHandlers.h"
#include <iostream>

int main (const int argc,const char ** argv)
{
	int errCode =0;
	HttpServer ::Server server;
	if( bindSignalHandlers(&server) )
	{
	   std::cout << "SignalHanlder binded..." <<std::endl;
	   errCode = server.run();
	   stopSignalHandlers();
	}
	return errCode;
}
