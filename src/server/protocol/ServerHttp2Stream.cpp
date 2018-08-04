
#include "ServerHttp2Stream.h"

#include "extensions/Sendfile.h"

namespace HttpServer
{
	ServerHttp2Stream::ServerHttp2Stream(
		Socket::Adapter &sock,
		const ServerSettings &settings,
		ServerControls &controls,
		Http2::IncStream *stream
	) noexcept
		: ServerHttp2Protocol(sock, settings, controls, stream)
	{

	}

	void ServerHttp2Stream::close() {
		this->stream->close();
	}

	ServerProtocol *ServerHttp2Stream::process()
	{
					return nullptr;
	}


}
