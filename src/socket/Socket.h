#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "../system/System.h"
#include <errno.h>
#include <iostream>

namespace Socket
{

	class Socket
	{
	protected:
			System::native_socket_type socket_handle;

	public:
			Socket() noexcept;
			Socket(const System::native_socket_type) noexcept;
			
			bool  tryBindPort(const int port);
			int static getLastError() noexcept;

			bool open() noexcept;
			bool close() noexcept;
			bool is_open() const noexcept;
			System::native_socket_type get_handle() const noexcept;

			bool bind(const int port)const  noexcept;
			bool listen()const  noexcept;

			Socket accept() const noexcept;
			
			bool nonblock(const bool flag = true)const noexcept;
			Socket &operator = (const Socket &obj) noexcept;

			bool operator == (const Socket &obj) const noexcept;
			bool operator != (const Socket &obj) const noexcept;
	};
}


#endif
