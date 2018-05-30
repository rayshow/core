#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/io/serialize.h>
#include<ccdk/io/filesystem/file.h>
#include<ccdk/io/io_module.h>

ccdk_namespace_io_start

using namespace ccdk::mpl;

class file_writer : public writer {

private:
	binary_file* pfile;
public:
	file_writer() noexcept : pfile{} {}
	file_writer(txt::string const& filename, binary_file::mode md) 
		:pfile{ new binary_file{filename, md} } {
		ccdk_assert(md != binary_file::eRead);
	}

	file_writer(binary_file& file) noexcept :pfile{ &file } {}

	virtual ptr::size_t write(const uint8* data, ptr::size_t size) override {
		ccdk_assert(pfile);
		return pfile->write(data, size);
	}

	virtual void rewind(rewind_mode mode = rewind_mode::current, uint32 offset = 0) {
		ccdk_assert(pfile);
		pfile->rewind(offset, (int)mode);
	}

	virtual void close() { if(pfile) pfile->close(); };
	virtual bool is_opened() { return pfile && pfile->is_opened(); }
	virtual bool reached_end() { return pfile && pfile->reached_end(); }
};

class file_reader : public reader {
private:
	binary_file * pfile;
public:
	file_reader() noexcept : pfile{} {}
	file_reader(txt::string const& filename, binary_file::mode md)
		:pfile{ new binary_file{ filename, md } } {
		ccdk_assert(md == binary_file::eRead);
	}

	file_reader(binary_file& file) noexcept : pfile{ &file } {}

	virtual ptr::size_t read(uint8* data, ptr::size_t size) override {
		ccdk_assert(pfile);
		return pfile->read(data, size);
	}

	virtual void rewind(rewind_mode mode = rewind_mode::current, uint32 offset = 0) override {
		ccdk_assert(pfile);
		pfile->rewind(offset, (int)mode);
	}

	virtual void close() override { if (pfile) pfile->close(); };
	virtual bool is_opened() override { return pfile && pfile->is_opened(); }
	virtual bool reached_end() override { return pfile && pfile->reached_end(); }
};


ccdk_namespace_io_end