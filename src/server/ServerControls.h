#ifndef __SERVERCONTROLS_H__
#define __SERVERCONTROLS_H__

#include "../system/Cache.h"
#include "../utils/Event.h"

#include <atomic>
#include <csignal>

namespace HttpServer
{
	class ServerControls
	{
	public:
		System::CachePaddingSize<sizeof(void *) * 3> padding_1;

		sig_atomic_t process_flag;
		sig_atomic_t restart_flag;

		Utils::Event* eventProcessQueue;
	public:
		ServerControls();
		~ServerControls();
		
		void clear();
		
		void setProcess(const bool flag = true);
		void stopProcess();
		void setRestart(const bool flag = true);
		void setProcessQueue();
	};
}

#endif
