#pragma once

#include<mutex>
#include<chrono>
#include<ctime>
#include<ccdk/mpl/design_pattern/singleton.h>
#include<ccdk/text/string.h>
#include<ccdk/io/io_module.h>
ccdk_namespace_io_start


using namespace mpl;

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


private:
	FILE* handle{};
	std::mutex mutex{};

public:
	bool open(const txt::string& filename) {
		if (handle != nullptr) {
			fclose(handle);
		}
		handle = fopen(filename.c_str(), "wt");
		if (!handle) {
			return false;
		}
		return true;
	}

	void close() { if (handle) { fclose(handle); } }
	bool is_opened() { return handle != nullptr; }

	void write( level lv, const txt::string& msg,
		const txt::string& file = "", uint32 line = 0) {
		
		txt::string log{};
		log.append(prefix(lv)).append(msg);
		if (lv >= level::Warn) log.append(suffix(file, line));
		log.append("\n");
		std::lock_guard<std::mutex> lock(mutex);
#if defined(CCDK_DEBUG)
		fprintf(stdout, log.c_str());
#else
		ccdk_assert(handle);
		fwrite(log.c_str(), 1, log.size(), handle);
#endif
	}


private:
	txt::string prefix(level lv) {
		using namespace std::chrono;
		
		constexpr static char* kLevelString[] = {
			"debug",
			"info ",
			"warn ",
			"error",
			"fatal"
		};

		auto now = system_clock::to_time_t(system_clock::now());
		auto tm = std::localtime(&now);
		char buffer[1024];
		sprintf(buffer, "[ %d-%2d-%2d %2.2d:%2.2d:%2.2d Level: %-5s]:", 
			tm->tm_year + 1900, tm->tm_mon,
			tm->tm_mday, tm->tm_hour, tm->tm_min,
			tm->tm_sec, kLevelString[(int)lv]);
		return buffer;
	}

	txt::string suffix(const txt::string& file, uint32 line) {
		txt::string str{ "{ from file : " };
		str.append(file).append(" line").append(line).append(" }");
	}

};

#define CCDK_DEBUG(msg)          logger::instance()->write(logger::level::Debug, msg,  __FILE__, __LINE__);
#define CCDK_INFO(msg)           logger::instance()->write(logger::level::Info,  msg,  __FILE__, __LINE__);
#define CCDK_WARNING(msg)        logger::instance()->write(logger::level::Warn,  msg,  __FILE__, __LINE__);
#define CCDK_ERROR(msg)          logger::instance()->write(logger::level::Error, msg,  __FILE__, __LINE__);
#define CCDK_FATAL(msg)          logger::instance()->write(logger::level::Fatal, msg,  __FILE__, __LINE__);
#define CCDK_LOG_FORMAT(lv, prefix,...) Logger::instance()->WriteFormat( (lv),   __FILE__, __LINE__, __VA_ARGS__ );

ccdk_namespace_io_end