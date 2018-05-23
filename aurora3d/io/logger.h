#pragma once

#include <string>
#include <mutex>
#include <chrono>
#include <ctime>
#include <module.h>
#include <ccdk/mpl/design_pattern/singleton.h>

a3d_namespace_io_start

using namespace ccdk;

class logger:public dp::singleton<logger>
{
public:
	enum class level :unsigned char {
		Debug = 0,
		Info,    
		Warn,    //report warning
		Error,   //report run-time error
		Fatal,   //exit immediately
		Max
	};
	static constexpr char* kLevelString[(int)level::Max] = {
		"debug",
		"info ",
		"warn ",
		"error",
		"fatal"
	};

private:
	FILE* handle{};
	std::mutex mutex{};
public:
	bool open(const std::string& filename) {
		if (handle != nullptr) {
			a3d_assert(0);
			fclose(handle);
		}
		handle = fopen(filename.c_str(), "wt");
		if (handle) {
			a3d_assert(0);
			return false;
		}
	}

	void write(level lv, const std::string& msg, const std::string& file = "", uint32 line = 0) {
		std::string log = prefix(lv) + msg;
		if (lv >= level::Warn) log += suffix(file, line);
		log += "\n";
		std::lock_guard<std::mutex> lock(mutex);
#if defined(A3D_DEBUG)
		fprintf(stdout, log.c_str());
#else
		fwrite(log.c_str(), 1, log.size(), handle);
#endif
	}

	void write(level lv, const std::wstring& msg, const std::wstring& file = WSTR(""), uint32 line = 0) {
		
	}

private:
	std::string prefix(level lv) {
		using namespace std::chrono;
		auto now = system_clock::to_time_t(system_clock::now());
		auto tm = std::localtime(&now);
		char buffer[1024];
		sprintf(buffer, "[ %d-%2d-%2d %2.2d:%2.2d:%2.2d Level: %-5s]:", tm->tm_year + 1900, tm->tm_mon,
			tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, kLevelString[(int)lv]);
		return buffer;
	}
	std::string suffix(const std::string& file, unsigned line) {
		return  "{ from file:" + file + " line:" + std::to_string(line) + " }";
	}

};

#define A3D_DEBUG(msg)          logger::instance()->write(logger::level::Debug, msg);
#define A3D_INFO(msg)           logger::instance()->write(logger::level::Info,  msg);
#define A3D_WARNING(msg)        logger::instance()->write(logger::level::Warn,  msg,  __FILE__, __LINE__);
#define A3D_ERROR(msg)          logger::instance()->write(logger::level::Error, msg,  __FILE__, __LINE__);
#define A3D_FATAL(msg)          logger::instance()->write(logger::level::Fatal, msg,  __FILE__, __LINE__);
#define A3D_LOG_FORMAT(lv, prefix,...) Logger::instance()->WriteFormat( (lv),   __FILE__, __LINE__, __VA_ARGS__ );

a3d_namespace_io_end