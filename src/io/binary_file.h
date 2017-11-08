#pragma once

#include<string>
#include"serialize.h"

namespace Aurora3D
{
	class BinaryFileReader :public Reader
	{
	private:
		size_type    file_size = 0;
		std::string  file_name{};
		uint32       check_sum = 0;
		FILE*        handle = nullptr;
	public:
		BinaryFileReader() = default;
		A3D_FORCEINLINE BinaryFileReader(const std::string& file){ Open(file); }
		virtual ~BinaryFileReader() { if (IsOpened()) Close(); }

		A3D_FORCEINLINE bool IsOpened() { return nullptr != handle; }
		A3D_FORCEINLINE void Close(){ if (IsOpened()) fclose(handle); }
		AURORA3D_API bool Open(const std::string& file);
		virtual bool ReadEnd() override { return read_pos >= file_size;  }
		virtual size_type Read(uint8* buf, size_type size) override;
		virtual bool SkipFromBegin(size_type skip) override;
		virtual bool SkipFromCurrent(size_type skip) override;
		virtual bool SkipFromEnd(size_type skip) override;
	};


	enum FileWriteMode :uint8
	{
		Replace = 0,
		Append
	};

	class BinaryFileWriter : public Writer
	{
	private:
		std::string file_name{};
		FILE*       handle = nullptr;
	public:
		BinaryFileWriter() = default;
		A3D_FORCEINLINE BinaryFileWriter(const std::string& file, FileWriteMode mode = FileWriteMode::Replace){ Open(file, mode); }
		virtual ~BinaryFileWriter() { if (IsOpened()){ Close();  } }

		A3D_FORCEINLINE bool IsOpened(){ return nullptr != handle; }
		A3D_FORCEINLINE void Close(){ if (IsOpened()) fclose(handle); }
		AURORA3D_API    bool Open(const std::string& file, FileWriteMode mode = FileWriteMode::Replace);
		virtual size_type Write(const uint8* data, size_type size) override ;
		virtual bool WritedEnd() override { return GetWritedSize() >= 0x7ffffffe; }
	};
	
}