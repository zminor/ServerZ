#ifndef __REQUESTDATA_H__
#define __REQUESTDATA_H__

#include <unordered_map>
#include <string>
#include "FileIncoming.h"
namespace Transfer
{
		struct request_data
		{
						std::unordered_multimap<std::string, std::string> incoming_headers;
						std::unordered_multimap<std::string, std::string> incoming_data;
						std::unordered_multimap<std::string, Transfer::FileIncoming> incoming_files;
		};
	
}


#endif
