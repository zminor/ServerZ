#ifndef __UTILS_H__
#define __UTILS_H__

#include <locale>
#include <vector>
#include <unordered_map>

namespace Utils
{
	uint8_t *packNumber(uint8_t *dest, const size_t number) noexcept;
	uint8_t *packString(uint8_t *dest, const std::string &str) noexcept;	
	
	void packNumber(std::vector<char> &buf, const size_t number);
	void packString(std::vector<char> &buf, const std::string &str);

	const uint8_t *unpackNumber(size_t *number, const uint8_t *src) noexcept;
	const uint8_t *unpackString(std::string &str, const uint8_t *src);

}


#endif

