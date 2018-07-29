#include "Socket.h"

namespace Socket
{
		Socket::Socket()noexcept: socket_handle(~0){}
		Socket::Socket(const System::native_socket_type fd) noexcept: socket_handle(fd){}

	  bool Socket:: tryBindPort(const int port)
		{
			if(open() == false)
			{
				std::cout << "Error: socket cannot be open; errno"<<Socket::Socket::getLastError()<<std::endl;
				return false;
			}
			return true;
		}

		int Socket::getLastError()
		{
			return errno;
		}
		
		bool Socket::open() noexcept
		{
		
		}

		bool Socket::bind(const int port) noexcept
		{
		
		}

		bool listen() noexcept
		{
		
		}

		bool nonblock(bool flag)
		{
		
		}


}
