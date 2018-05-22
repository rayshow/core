
#include<cassert>

#include"../constant_buffer.h"
#include"../graphics.h"
#include"graphics_dx11_impl.h"


namespace Aurora3D
{
	void ConstantBuffer::OnDeviceLost()
	{
		///no-op
	}
	
	void ConstantBuffer::OnDeviceReset()
	{
		///no-op
	}

	bool ConstantBuffer::SetSize(unsigned size)
	{
		Release();
		assert(size != 0);
		if (!size) return false;

		size += 15;
		size &= 0xfffffff0;

		size_ = size;
		dirty_ = false;
		cpu_data_.assign(size, 0);

		auto graphics_ptr = graphics_.lock();
		if (graphics_ptr)
		{
			D3D11_BUFFER_DESC buffer_desc;
			memset(&buffer_desc, 0, sizeof(buffer_desc));
			buffer_desc.ByteWidth = size;
			buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			buffer_desc.CPUAccessFlags = 0;
			buffer_desc.Usage = D3D11_USAGE_DEFAULT;

		}
		return true;
	}
	

	void ConstantBuffer::Commit()
	{
		
	}

	void ConstantBuffer::Release()
	{
		DX_SAFE_RELEASE(object_.ptr);
		cpu_data_.swap(std::vector<unsigned char>());
		size_ = 0;
	}
}