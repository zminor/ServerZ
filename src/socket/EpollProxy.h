#ifndef __EPOLLPROXY_H__
#define __EPOLLPROXY_H__

#include <vector>
#include <sys/epoll.h>
#include "Socket.h"

namespace Socket
{
	class EpollProxy
	{
	protected:
		int _epfd;
		mutable std::vector <struct ::epoll_event> epoll_events;

	public:
		EpollProxy() noexcept;
		EpollProxy(EpollProxy &&obe) noexcept;
		~EpollProxy() noexcept;

		bool create(const size_t startListSize =1);
		bool create1(const int flag =0);

		void destroy()noexcept;

		bool is_created() const noexcept;
	
		bool addSocket(const Socket &sock) noexcept;
		bool removeSocket(const Socket &sock) noexcept;

		bool accept(std::vector <Socket> &sockets) const noexcept;
		
	};

}




#endif
