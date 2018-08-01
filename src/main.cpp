#include "server/Server.h"

int main (const int argc,const char ** argv)
{
	int errCode =0;
	HttpServer ::Server Server;
	errCode = Server.run();
	return 0;
}
