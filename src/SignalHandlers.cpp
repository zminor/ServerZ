
#include "SignalHandlers.h"
#include "system/System.h"
#include <csignal>

static HttpServer::Server *globalServerPtr = nullptr;

/**
 * Terminate signal
 */
static void handlerSigTerm(const int) noexcept {
	if (globalServerPtr) {
		globalServerPtr->stop();
	}
}

/**
 * Interrupt signal
 */
static void handlerSigInt(const int) noexcept {
	if (globalServerPtr) {
		globalServerPtr->stop();
	}
}

/**
 * Signal to restart

static void handlerSigUsr1(const int) noexcept {
	if (globalServerPtr) {
		globalServerPtr->restart();
	}
}
*/

/**
 * Signal to update modules

static void handlerSigUsr2(const int) noexcept {
	if (globalServerPtr) {
		globalServerPtr->update();
	}
}
*/

bool bindSignalHandlers(HttpServer::Server *server) noexcept
{
	globalServerPtr = server;

	struct ::sigaction act {}; 

	act.sa_handler = handlerSigInt;
	::sigaction(SIGINT, &act, nullptr);//Interrupt //

	act.sa_handler = handlerSigTerm;
	::sigaction(SIGTERM, &act, nullptr);//Terminate

/*
	act.sa_handler = handlerSigUsr1;
	::sigaction(SIGUSR1, &act, nullptr);//Restart

	act.sa_handler = handlerSigUsr2;
	::sigaction(SIGUSR2, &act, nullptr);//updateModule
*/
	::signal(SIGPIPE, SIG_IGN);//Ignore to avoid program quit
	return true;
}

void stopSignalHandlers() noexcept
{

}
