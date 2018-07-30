#include "EpollProxy.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace Socket
{
	EpollProxy::EpollProxy() noexcept: _epfd(~0){}

	EpollProxy::EpollProxy(EpollProxy &&obj) noexcept: _epfd(obj._epfd)
	{
		obj._epfd = ~0;		
		obj.epoll_events.swap( this->epoll_events);
	}
	EpollProxy::~EpollProxy() noexcept
	{
		this->destroy();
	}
	
	bool EpollProxy :: create1(const int flag)
	{
		this->destroy();
		::epoll_create1(flag);
		if(this->_epfd == ~0)
		{
			return false;	
		}

		return true;
	}
	bool EpollProxy :: create(const size_t startListSize)
	{
		this->destroy();

		_epfd = ::epoll_create(startListSize);
		if(this->_epfd == ~0)
		{
			return false;
		}
		if(startListSize >0)
		{
			this->epoll_events.reserve(startListSize);
		}
		printf("Epoll %d ready...\n", this->_epfd);
		return true;
	}

	void EpollProxy :: destroy()noexcept
	{
		if(this->is_created())
		{
			::close(this->_epfd);
			this->_epfd = ~0;
			this->epoll_events.clear();
		}
	}

	bool EpollProxy::is_created() const noexcept
	{
		return this->_epfd != ~0;
	}

	bool EpollProxy::addSocket(const Socket &sock) noexcept
	{
		if(this->is_created() == false)
		{
			return false;
		}
		struct ::epoll_event event
		{
			EPOLLIN || EPOLLET || EPOLLRDHUP,
			reinterpret_cast <void*> (sock.get_handle())
		};

		const int result = ::epoll_ctl(
				this->_epfd,
				EPOLL_CTL_ADD,
				sock.get_handle(),
				&event
				);
		if(result == ~0)
		{
			return false;
		}
		this->epoll_events.emplace_back(event);
		return true;
	}

	bool EpollProxy::removeSocket(const Socket &sock) noexcept
	{
		if (this->is_created() == false) {
			return false;
		}


		const int result = ::epoll_ctl(
			this->_epfd,
			EPOLL_CTL_DEL,
			sock.get_handle(),
			nullptr
		);

		if (result == ~0) {
			return false;
		}

		this->epoll_events.pop_back();

		return true;
	}

	bool EpollProxy::accept(Socket &sock) const noexcept
	{
		if(this->is_created())
		{
			const int count = ::epoll_wait(
					this->_epfd,
					this->epoll_events.data(),
					this->epoll_events.size(),
					~0
					);
			if(count == ~0)
			{
				return false;
			}

			for(int i=0; i< count; ++i)
			{
				const epoll_event &event = this->epoll_events[i];
				if(event.events & EPOLLIN)
				{
					System::native_socket_type client_socket = ~0;

					do{
						client_socket = ::accept(
								event.data.fd,
								static_cast<sockaddr *> (nullptr),
								static_cast <socklen_t*>(nullptr)
								);
						
					}
					while(~0 != client_socket);
				}
			}
		}
		return true;
	}

}
