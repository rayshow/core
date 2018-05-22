#pragma once

#include<string>
#include<module.h>
#include<io/serialize.h>
#include<io/logger.h>

a3d_namespace_io_start

class binary_reader :public reader
{
private:
	ptr::size_t  size = 0;
	std::string  name{};
	uint32       check_sum = 0;
	FILE*        handle = nullptr;
public:

	virtual ~binary_reader() { if (is_opened()) close(); }

	CCDK_FORCEINLINE binary_reader() = default;
	CCDK_FORCEINLINE binary_reader(const std::string& file){ open(file); }
	
	CCDK_FORCEINLINE void close() { if (is_opened()) fclose(handle); }
	
	CCDK_FORCEINLINE void open(std::string const& filename) {
		if (is_opened()) close();
		return open_impl(filename);
	}

	CCDK_FORCEINLINE virtual bool is_opened() override { return nullptr != handle; }
	
	virtual bool is_end() override { return pos() >= size;  }

	virtual ptr::size_t read(uint8* buf, ptr::size_t size) override {

	}
	virtual bool skip_from_begin(ptr::size_t n) override {

	}
	virtual bool skip_from_cur(ptr::size_t n) override {

	}
	virtual bool skip_from_end(ptr::size_t n) override {

	}

private:

	void open_impl(std::string const& filename) {
		a3d_assert(!filename.empty());
		std::string path = PathHelper::UnixStylePath(filename);

		if (nullptr == (handle = fopen(path.c_str(), "rb")))
		{
			A3D_WARNING("file " + path + " open failed, please ensure file name is correct.");
			return false;
		}
		fseek(handle, 0, SEEK_END);
		ptr::size_t tmpsz = ftell(handle);
		fseek(handle, 0, SEEK_SET);
		if (0 == tmpsz)
		{
			close();
			A3D_WARNING("file " + path + " is empty");
			return false;
		}

		name = path;
		size = tmpsz;
		return true;
	}

};




class BinaryFileWriter : public writer
{
private:
	std::string file_name{};
	FILE*       handle = nullptr;
public:
	BinaryFileWriter() = default;
	CCDK_FORCEINLINE BinaryFileWriter(const std::string& file, FileWriteMode mode = FileWriteMode::Replace){ Open(file, mode); }
	virtual ~BinaryFileWriter() { if (IsOpened()){ Close();  } }

	CCDK_FORCEINLINE bool IsOpened(){ return nullptr != handle; }
	CCDK_FORCEINLINE void Close(){ if (IsOpened()) fclose(handle); }
	AURORCCDK_API    bool Open(const std::string& file, FileWriteMode mode = FileWriteMode::Replace);
	virtual size_type Write(const uint8* data, size_type size) override ;
	virtual bool WritedEnd() override { return GetWritedSize() >= 0x7ffffffe; }
};

a3d_namespace_io_end