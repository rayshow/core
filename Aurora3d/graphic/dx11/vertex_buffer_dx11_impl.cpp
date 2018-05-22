#include"../vertex_buffer.h"
#include"../graphics.h"
#include"graphics_dx11_impl.h"

namespace Aurora3D
{
	VertexBuffer::~VertexBuffer()
	{
	}

	void VertexBuffer::OnDeviceLost()
	{

	}
	void VertexBuffer::OnDeviceReset()
	{

	}
	void VertexBuffer::Release()
	{

	}

	void VertexBuffer::UnmapBuffer()
	{
		auto gs = graphics_.lock();
		if (gs && object_.ptr && lock_state_ == LockState::Hardware)
		{
			/*gs->GetGraphicsImpl()->GetDeviceContext()->Unmap((ID3D11Buffer*)object_.ptr,
				0);*/
			lock_state_ = LockState::Hardware;
		}
	}

	void VertexBuffer::Unlock()
	{
		switch (lock_state_)
		{
		case LockState::Hardware:
			UnmapBuffer();
			break;
		case LockState::Shadow:
			break;
		case LockState::Scratch:
			break;
		default:
			break;
		}
	}

}