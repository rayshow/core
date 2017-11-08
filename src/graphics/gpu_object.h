#pragma once

#include<memory>
#include"graphics_def.h"

namespace Aurora3D
{
	union GPUObjectHandle
	{
		void*    ptr ;
		unsigned name;
	};

	class GPUObject: public std::enable_shared_from_this<GPUObject>
	{
	protected:
		std::weak_ptr< Graphics > graphics_{};
		GPUObjectHandle object_{};
		bool is_lost_ = false;
		bool is_pending_ = false;
	public:
		GPUObject(GraphicsPtr graphics);
		virtual ~GPUObject();

		///dx gpu object behavior
		virtual void OnDeviceLost();
		virtual void OnDeviceReset();
		virtual void Release();
		void ClearDataLost();

		///operation
		void* GetGPUObject() const { return object_.ptr; }
		unsigned GetGPUObjectName() const { return object_.name; }
	};
}
