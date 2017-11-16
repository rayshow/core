#pragma once

#include<memory>
#include"../common_def.h"


namespace Aurora3D
{
	class Logger;
	class LogEntry;
	class Buffer;
	template<size_type size>
	class FixBuffer;
	class MappingBuffer;
	class BinaryFile;

	typedef std::shared_ptr<Logger>			LoggerPtr;
	typedef std::shared_ptr<LogEntry>		LoggerEntryPtr;
	typedef std::shared_ptr<Buffer>			BufferPtr;
	typedef std::shared_ptr<MappingBuffer>  MappingBufferPtr;
	typedef std::shared_ptr<BinaryFile>     BinaryFilePtr;
}