#include "Utils.h"

#include <array>
#include <chrono>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <cctype>
#include <algorithm>

namespace Utils
{
	constexpr uint8_t PACK_NUMBER_SIZE_BYTE = 252;
	constexpr uint8_t PACK_NUMBER_SIZE_16 = 253;
	constexpr uint8_t PACK_NUMBER_SIZE_32 = 254;
	constexpr uint8_t PACK_NUMBER_SIZE_MAX = 255;

	uint8_t *packNumber(uint8_t *dest, const size_t number) noexcept
	{
		if (number <= PACK_NUMBER_SIZE_BYTE) {
			*dest = static_cast<uint8_t>(number);

			dest += sizeof(uint8_t);
		}
		else if (number <= std::numeric_limits<uint16_t>::max() ) {
			*dest = PACK_NUMBER_SIZE_16;

			dest += sizeof(uint8_t);

			*reinterpret_cast<uint16_t *>(dest) = static_cast<uint16_t>(number);

			dest += sizeof(uint16_t);
		}
		else if (number <= std::numeric_limits<uint32_t>::max() ) {
			*dest = PACK_NUMBER_SIZE_32;

			dest += sizeof(uint8_t);

			*reinterpret_cast<uint32_t *>(dest) = static_cast<uint32_t>(number);

			dest += sizeof(uint32_t);
		} else {
			*dest = PACK_NUMBER_SIZE_MAX;

			dest += sizeof(uint8_t);

			*reinterpret_cast<size_t *>(dest) = number;

			dest += sizeof(size_t);
		}

		return dest;
	}

	uint8_t *packString(uint8_t *dest, const std::string &str) noexcept {
		dest = packNumber(dest, str.length() );
		std::memcpy(dest, str.data(), str.length() );
		return dest + str.length();
	}

	void packNumber(std::vector<char> &buf, const size_t number)
	{
		if (number <= PACK_NUMBER_SIZE_BYTE) {
			buf.emplace_back(number);
		}
		else if (number <= std::numeric_limits<uint16_t>::max() ) {
			buf.emplace_back(PACK_NUMBER_SIZE_16);

			buf.resize(buf.size() + sizeof(uint16_t) );

			*reinterpret_cast<uint16_t *>(buf.data() + buf.size() - sizeof(uint16_t) ) = static_cast<uint16_t>(number);
		}
		else if (number <= std::numeric_limits<uint32_t>::max() ) {
			buf.emplace_back(PACK_NUMBER_SIZE_32);

			buf.resize(buf.size() + sizeof(uint32_t) );

			*reinterpret_cast<uint32_t *>(buf.data() + buf.size() - sizeof(uint32_t) ) = static_cast<uint32_t>(number);
		} else {
			buf.emplace_back(PACK_NUMBER_SIZE_MAX);

			buf.resize(buf.size() + sizeof(size_t) );

			*reinterpret_cast<size_t *>(buf.data() + buf.size() - sizeof(size_t) ) = number;
		}
	}

	void packString(std::vector<char> &buf, const std::string &str)
	{
		packNumber(buf, str.length() );

		if (str.length() ) {
			buf.insert(
				buf.end(),
				str.cbegin(),
				str.cend()
			);
		}
	}

const uint8_t *unpackNumber(size_t *number, const uint8_t *src) noexcept
	{
		*number = *src;
		src += sizeof(uint8_t);
		if (*number <= PACK_NUMBER_SIZE_BYTE) {

		}
		else if (*number == PACK_NUMBER_SIZE_16) {
			*number = *reinterpret_cast<const uint16_t *>(src);
			src += sizeof(uint16_t);
		}
		else if (*number == PACK_NUMBER_SIZE_32) {
			*number = *reinterpret_cast<const uint32_t *>(src);
			src += sizeof(uint32_t);
		} 
		else {
			*number = *reinterpret_cast<const size_t *>(src);
			src += sizeof(size_t);
		}

		return src;
	}

	const uint8_t *unpackString(std::string &str, const uint8_t *src)
	{
		size_t length;
		src = unpackNumber(&length, src);

		str.assign(src, src + length);

		return src + length;
	}
}
