#include <ctime>
#include <cassert>
#include <chrono>
#include <ratio>
#include <cstdio>
#include <cstdarg>
#include "logger.h"
#include "../string/astring.h"

#pragma warning(disable:4996)

namespace Aurora3D
{
	static std::string ksLevel[(int)LogLevel::Max]=
	{
		"debug",
		"info",
		"warn",
		"error",
		"fatal"
	};

	std::string Logger::GetPrefix(LogLevel lv)
	{
		using namespace std::chrono;
		auto now = system_clock::to_time_t(system_clock::now());
		auto tm = std::localtime(&now);

		return StringFormater<128>::Format("[ %d-%2d-%2d %2.2d:%2.2d:%2.2d Level: %-5s]:",
			tm->tm_year + 1900, tm->tm_mon,
			tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, ksLevel[(int)lv].c_str());
	}

	void Logger::Write(LogLevel lv, const WString& msg, const WString& source_file, uint32 line)
	{

	}

	void Logger::Write(LogLevel lv, const AString& msg,
		const AString& source_file, unsigned line)
	{
		AString log = GetPrefix(lv)+msg;
		if (lv >= LogLevel::Warn)  log += GetSuffix(source_file, line);
		log += "\n";
		std::lock_guard<std::mutex> lock(mutex);
		file.Write((const uint8*)log.c_str(), log.length());

#ifdef AURORA3D_DEBUG 
		//debug mode to console also
		fprintf(stdout, log.c_str());
#endif
	}

}