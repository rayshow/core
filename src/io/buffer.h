#pragma once

#include<cassert>
#include<string>

#include"../common_def.h"
#include"serialize.h"

namespace Aurora3D
{
	class RWBuffer:public Writer
	{
	protected:
		uint8*    buffer = nullptr;
		size_type buffer_size = 0;
	public:
		RWBuffer() = default;
		A3D_FORCEINLINE RWBuffer(uint8* memaddr, size_type memsize) :
			buffer(memaddr), buffer_size(memsize) { }
		virtual ~RWBuffer() { buffer = nullptr;}

		A3D_FORCEINLINE bool IsValid() { return nullptr != buffer; }
		A3D_FORCEINLINE uint8& operator[](int32 index) { return buffer[index]; }
		A3D_FORCEINLINE const uint8* GetBuffer() const { return buffer; }
		A3D_FORCEINLINE size_type GetBufferSize() const { return buffer_size; }

		virtual bool WritedEnd() override { return GetWritedSize() >= buffer_size; }
		virtual size_type Write(const uint8* data, size_type size) override;
	};


	template< size_type kSize >
	class FixBuffer :public RWBuffer
	{
	private:
		uint8 memory[kSize]{};
	public:
		FixBuffer() :RWBuffer(memory, kSize){}
	};


	class MappingBuffer:public RWBuffer
	{
	public:
		MappingBuffer(const std::string& file);
		virtual ~MappingBuffer(){
			if (buffer){
				delete[] buffer;
			}
		}
	};


}