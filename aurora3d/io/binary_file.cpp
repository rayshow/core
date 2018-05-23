#include<cstdio>
#include"logger.h"
#include"binary_file.h"
#include"file_system.h"

#pragma warning(disable: 4996)

namespace Aurora3D
{
	bool BinaryFileReader::Open(const std::string& file)
	{
		
	}

	size_type BinaryFileReader::Read(uint8* buf, size_type size)
	{
		return fread(buf, size, 1, handle);
	}
	bool BinaryFileReader::SkipFromBegin(size_type skip)
	{
		CCDK_ASSERT(skip <= file_size);
		if (skip > file_size) return false;
		read_pos = skip;
		fseek(handle, read_pos, SEEK_SET);
		return true;
	}
	bool BinaryFileReader::SkipFromCurrent(size_type skip)
	{
		CCDK_ASSERT(read_pos + skip <= file_size);
		if (read_pos + skip > file_size) return false;
		read_pos += skip;
		fseek(handle, read_pos, SEEK_SET);
		return true;
	}
	bool BinaryFileReader::SkipFromEnd(size_type skip)
	{
		CCDK_ASSERT(skip <= file_size);
		if (skip > file_size) return false;
		read_pos = file_size - skip;
		fseek(handle, read_pos, SEEK_SET);
		return true;
	}

	

	bool BinaryFileWriter::Open(const AString& file, FileWriteMode mode)
	{
		

	}
	size_type BinaryFileWriter::Write(const uint8* data, size_type size)
	{
		return 
	}
}