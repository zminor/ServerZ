#ifndef __SOCKET_H__
#define __SOCKET_H__
#include "../system/System.h"
namespace Socket
{

	class Socket
	{
	protected:
			System::native_socket_type socket_handle;

	public:
			Socket() noexcept;
			Socket(const System::native_socket_type) noexcept;
			
			bool tryBindPort(const int port);
			int Socket::getLastError() noexcept;

			bool open() noexcept;
			bool bind(const int port) noexcept;
			bool listen() noexcept;
			bool nonblock(bool flag = true) noexcept;
	};
}


#endif
