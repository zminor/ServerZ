#include "ServerControls.h"

namespace HttpServer
{

	ServerControls::~ServerControls()
	{
					this->clear();
	}

	void ServerControls::clear()
	{

	}

	void ServerControls::setProcess(const bool flag)
	{
					this->process_flag = flag;
	}

	void ServerControls::stopProcess()
	{
					this->process_flag = false;
					
					this->setProcessQueue();
	}
	
	void ServerControls::setRestart(const bool flag)
	{					
					this->restart_flag = flag;
	}

	void ServerControls::setProcessQueue()
	{
	}
}
