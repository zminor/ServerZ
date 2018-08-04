
#include "System.h"

#include <array>

	#include <csignal>
	#include <sys/sysinfo.h>
	#include <sys/stat.h>
	#include <unistd.h>


namespace System
{
	native_processid_type getProcessId() noexcept
	{
		return ::getpid();
	}

	bool changeCurrentDirectory(const std::string &dir)
	{	
		return 0 == ::chdir(dir.c_str() );
	}

	bool isProcessExists(const native_processid_type pid) noexcept
	{
		return 0 == ::kill(pid, 0); //Checkout if pid exist
	}

	bool sendSignal(const native_processid_type pid, const int signal) noexcept
	{
		return 0 == ::kill(pid, signal);
	}

	bool isDoneThread(const std::thread::native_handle_type handle) noexcept
	{
		return 0 != ::pthread_kill(handle, 0);
	}

	std::string getTempDir()
	{
		const char *buf = ::getenv("TMPDIR");

		if (nullptr == buf) {
			return std::string("/tmp/");
		}

		std::string str(buf);

		if (str.back() != '/') {
			str.push_back('/');
		}

		return str;
	}

	bool isFileExists(const std::string &fileName)
	{
		struct ::stat attrib;

		if (-1 == ::stat(fileName.c_str(), &attrib) ) {
			return false;
		}

		return S_ISREG(attrib.st_mode);
	}

	bool getFileSizeAndTimeGmt(
		const std::string &filePath,
		size_t *fileSize,
		time_t *fileTime
	) {
		struct ::stat attrib {};

		if (-1 == ::stat(filePath.c_str(), &attrib) ) {
			return false;
		}

		*fileSize = static_cast<size_t>(attrib.st_size);

		std::tm clock {};

		::gmtime_r(&(attrib.st_mtime), &clock);

		*fileTime = std::mktime(&clock);

		return true;
	}

	void filterSharedMemoryName(std::string &memName)
	{
		if (memName.front() != '/') {
			memName = '/' + memName;
		}

		for (size_t i = 1; i < memName.length(); ++i) {
			if ('/' == memName[i] || '\\' == memName[i]) {
				memName[i] = '-';
			}
		}
	}
}
