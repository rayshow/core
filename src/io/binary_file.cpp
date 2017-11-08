#include<cstdio>
#include"logger.h"
#include"binary_file.h"
#include"file_system.h"

#pragma warning(disable: 4996)

namespace Aurora3D
{
	bool BinaryFileReader::Open(const std::string& file)
	{
		A3D_ASSERT(!file.empty());

		if (IsOpened())
		{
			A3D_WARNING(file + " had been opened, close it and open again");
			return false;
		}

		std::string& path = PathHelper::UnixStylePath(file);

		if (nullptr == ( handle = fopen(path.c_str(), "rb")))
		{
			A3D_WARNING("file "+ path + " open failed, please ensure file name is correct.");
			return false;
		}
		fseek(handle, 0, SEEK_END);
		long size = ftell(handle);
		fseek(handle, 0, SEEK_SET);
		if (0 == size)
		{
			Close();
			A3D_WARNING("file " + path + " is empty");
			return false;
		}

		file_name = path;
		file_size = size;
		return true;
	}

	size_type BinaryFileReader::Read(uint8* buf, size_type size)
	{
		return fread(buf, size, 1, handle);
	}
	bool BinaryFileReader::SkipFromBegin(size_type skip)
	{
		A3D_ASSERT(skip <= file_size);
		if (skip > file_size) return false;
		read_pos = skip;
		fseek(handle, read_pos, SEEK_SET);
		return true;
	}
	bool BinaryFileReader::SkipFromCurrent(size_type skip)
	{
		A3D_ASSERT(read_pos + skip <= file_size);
		if (read_pos + skip > file_size) return false;
		read_pos += skip;
		fseek(handle, read_pos, SEEK_SET);
		return true;
	}
	bool BinaryFileReader::SkipFromEnd(size_type skip)
	{
		A3D_ASSERT(skip <= file_size);
		if (skip > file_size) return false;
		read_pos = file_size - skip;
		fseek(handle, read_pos, SEEK_SET);
		return true;
	}

	CONSTEXPR achar* kWriteMode[2] = { "rb", "r+b" };

	bool BinaryFileWriter::Open(const AString& file, FileWriteMode mode)
	{
		A3D_ASSERT(!file.empty());

		if (IsOpened())
		{
			A3D_WARNING(file + " had been opened, close it and open again");
			return false;
		}
		std::string& path = PathHelper::UnixStylePath(file);
		if (nullptr == (handle = fopen(path.c_str(), kWriteMode[uint32(mode)] )))
		{
			A3D_WARNING("file " + path + " open failed, please ensure file name is correct.");
			return false;
		}
		file_name = path;
		return true;

	}
	size_type BinaryFileWriter::Write(const uint8* data, size_type size)
	{
		return fwrite(data, size, 1, handle);
	}
}