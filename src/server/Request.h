#ifndef __REQUEST_H__
#define __REQUEST_H__

#include "protocol/ServerProtocol.h"
#include "../transfer/FileIncoming.h"
#include "../transfer/ProtocolVariant.h"

#include <unordered_map>

namespace HttpServer
{
	struct Request
	{
	//	ServerProtocol *prot;
		std::string document_root;
		std::string host;
		std::string path;
		std::string method;
		std::unordered_multimap<std::string, std::string> params;
		std::unordered_multimap<std::string, std::string> headers;
		std::unordered_multimap<std::string, std::string> data;
		std::unordered_multimap<std::string, Transfer::FileIncoming> files;
		std::unordered_multimap<std::string, std::string> cookies;

		Transfer::ProtocolVariant protocol_variant;

	public:
		std::string getHeader(const std::string &key) const;

		bool isDataExists(const std::string &key) const;

		std::string getDataAsString(const std::string &key) const;

		std::vector<std::string> getDataAsArray(const std::string &key) const;

		bool isFileExists(const std::string &key) const;

		Transfer::FileIncoming getFile(const std::string &key) const;

		std::vector<Transfer::FileIncoming> getFilesAsArray(const std::string &key) const;

		std::string getCookieAsString(const std::string &cookieName) const;
	};
}

#endif
