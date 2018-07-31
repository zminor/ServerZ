#include "Socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

namespace Socket
{
		Socket::Socket()noexcept: socket_handle(~0){}
		Socket::Socket(const System::native_socket_type fd) noexcept: socket_handle(fd){}

	  bool Socket:: tryBindPort(const int port)
		{
		if (open() == false) {
			std::cout << "Error: socket cannot be open; errno " << Socket::Socket::getLastError() << ";" << std::endl;
			return false;
		}

		if (bind(port) == false) {
			std::cout << "Error: port " << port << " cannot be bind; errno " << Socket::Socket::getLastError() << ";" << std::endl;
			return false;
		}

		if (listen() == false) {
			std::cout << "Error: socket " << port << " cannot be listen; errno " << Socket::Socket::getLastError() << ";" << std::endl;
			return false;
		}
		
		nonblock(true);
		return true;
		}

		int Socket::getLastError() noexcept
		{
			return errno;
		}
		
		bool Socket::is_open() const noexcept
		{
			return ~0 != this->socket_handle;
		}

		System::native_socket_type Socket::get_handle() const noexcept
		{
			return this->socket_handle;
		}

		bool Socket::open() noexcept
		{
			this->close();
			this->socket_handle = ::socket(AF_INET, SOCK_STREAM,0);
			return this->is_open();
		}
		
		bool Socket::close() noexcept
		{
			if(this-> is_open())
			{
				const int result = ::close(this->socket_handle);
				if( 0 == result)
				{
					this->socket_handle = ~0;
					return true;
				}
			}
			return false;
		}

		bool Socket::bind(const int port)const noexcept
		{
			const ::sockaddr_in sock_addr
			{
				AF_INET,
				htons(port),
				htonl(INADDR_ANY),
				0
			};
	
			return 0 == ::bind(
					this->socket_handle,
					reinterpret_cast<const sockaddr *> (&sock_addr),
					sizeof(sockaddr_in)
					);
		}

		bool Socket::listen()const  noexcept
		{
			return 0 == ::listen(this->socket_handle, SOMAXCONN);
		}
		
		bool List::accept(std::vector<Socket> &sockets) const noexcept
		{
			if (this->is_created() )
			{
				const int count = ::epoll_wait(
				this->obj_list,
				this->epoll_events.data(),
				this->epoll_events.size(),
				~0
			);

			if (count == ~0)
		       	{
				return false;
			}

			for (int i = 0; i < count; ++i)
			{
				const epoll_event &event = this->epoll_events[i];
				if (event.events & EPOLLIN)
				{
					System::native_socket_type client_socket = ~0;

					do {
						client_socket = ::accept(
							event.data.fd,
							static_cast<sockaddr *>(nullptr),
							static_cast<socklen_t *>(nullptr)
						);
						if (~0 != client_socket) {
							sockets.emplace_back(Socket(client_socket) );
						}
					}
					while (~0 != client_socket);
				}
			}
			return false == sockets.empty();
		}
		return false;
	}

		bool Socket:: nonblock(bool flag) const noexcept
		{
			return ~0 != ::fcntl(
				this->socket_handle,
				F_SETFL,
				flag ? O_NONBLOCK: O_SYNC
				);
		}

		Socket& Socket ::operator = (const Socket &obj) noexcept
		{
			this->socket_handle = obj.socket_handle;
			return *this;
		}

		bool Socket:: operator == (const Socket &obj) const noexcept
		{
			return obj.socket_handle == this->socket_handle;
		}

		bool Socket:: operator != (const Socket &obj) const noexcept
		{
			return obj.socket_handle != this->socket_handle;
		}

}
