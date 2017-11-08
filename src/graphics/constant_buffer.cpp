#include"constant_buffer.h"

namespace Aurora3D
{
	ConstantBuffer::~ConstantBuffer()
	{
		Release();
	}

	bool ConstantBuffer::UpdateData(unsigned offset, unsigned size, const void* data)
	{
		if (offset + size > size_) return false;
		memcpy(&cpu_data_[offset], data, size);
		dirty_ = true;
		return true;
	}
}