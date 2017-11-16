#pragma once

#include <string>
#include <mutex>
#include <list>

#include "../design/singleton.h"
#include "../precompile_def.h"
#include "binary_file.h"
#include "buffer.h"

namespace Aurora3D
{
	enum class LogLevel :unsigned char
	{
		Debug = 0,
		Info,
		Warn,
		Error,
		Fatal,
		Max
	};

	class Logger:public Singleton<Logger> 
	{
	private:
		BinaryFileWriter file{};
		std::mutex mutex{};
		AString GetPrefix(LogLevel lv);
		AString GetSuffix(const AString& source_file, unsigned line)
		{
			return  "( from file:" + source_file + " line:" + std::to_string(line) + " )";
		}

	public:
		Logger(){}
		bool Open(const AString& log_name)
		{
	
			return file.Open(log_name, FileWriteMode::Append);
		}

		void Write(LogLevel lv, const AString& msg, const AString& source_file = "", uint32 line=0);
		void Write(LogLevel lv, const WString& msg, const WString& source_file = WSTR(""), uint32 line = 0);

		template<typename Ch, typename S>
		void WriteFormat(LogLevel lv, const S & source_file, unsigned line, const Ch *fmt, prefix,...)
		{
			static_assert(CharType<Ch>::IsCharType, "format string must be const char or wchar literal!");
			S ret = StringFormater::Format<0>(fmt, prefix,...);
			Write(lv, ret, source_file, line);
		}
	};

#ifdef AURORCCDK_DEBUG
#	define CCDK_DEBUGGER(msg)         Logger::GetSingletonPtr()->Write(LogLevel::Debug, msg);
#	define CCDK_ASSERT(state)	     assert(state);           
#	define CCDK_CHECK_UNLIKELY(expr)  if(UNLIKELY(!(state)){ Logger::GetSingletonPtr()->WriteFormat(LogLevel::Warn,  __FILE__, __LINE__, "express unlikely happened:",#expr ); };
#else 
#	define CCDK_DEBUGGER(msg) 
#	define CCDK_ASSERT(state)     if(state) Logger::GetSingletonPtr()->Write(LogLevel::Error, "Assert Failed!", __FILE__, __LINE__);
#	define CCDK_CHECK_UNLIKELY(expr)  
#endif

#define CCDK_INFO(msg)           Logger::GetSingletonPtr()->Write(LogLevel::Info, msg);
#define CCDK_WARNING(msg)        Logger::GetSingletonPtr()->Write(LogLevel::Warn, msg,  AString{__FILE__}, __LINE__);
#define CCDK_ERROR(msg)          Logger::GetSingletonPtr()->Write(LogLevel::Error, msg, AString{__FILE__}, __LINE__);
#define CCDK_FATAL(msg)          Logger::GetSingletonPtr()->Write(LogLevel::Fatal, msg, AString{__FILE__}, __LINE__);
#define CCDK_LOG_FORMAT(lv, prefix,...) Logger::GetSingletonPtr()->WriteFormat( (lv),  AString{__FILE__}, __LINE__, __VA_ARGS__ );
};
