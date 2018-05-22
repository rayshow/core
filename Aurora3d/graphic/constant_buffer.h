#pragma once

#include<vector>
#include"gpu_object.h"

namespace Aurora3D
{
	class ConstantBuffer: public GPUObject
	{
	private:
		std::vector<unsigned char> cpu_data_;
		unsigned size_ = 0;
		bool dirty_ = false;
	public:
		virtual ~ConstantBuffer();

		///gpu device process
		virtual void OnDeviceLost() override;
		virtual void OnDeviceReset() override;
		virtual void Release() override;

		///allocate object
		bool SetSize(unsigned size);
		bool UpdateData(unsigned offset, unsigned size, const void* data);
		void Commit();

		///read parameter
		unsigned GetSize() const { return size_;  }
		bool IsDirty() const { return dirty_;  }
	};
}