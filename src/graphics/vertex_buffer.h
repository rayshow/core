#pragma once

#include<vector>
#include"gpu_object.h"
#include"graphics_def.h"

namespace Aurora3D
{
	enum class VertexElementType :unsigned char
	{
		Int = 0,
		Float,
		Vector2,
		Vector3,
		Vector4,
		UByte4,
		UByte4Norm,
		Matrix3,
		Matrix3x4,
		Matrix4,
		Max
	};

	enum class VertexSemantic :unsigned char
	{
		Position = 0,
		Normal,
		Binormal,
		Tangent,
		Texcoord,
		Texcoord2,
		Color,
		BlendWeights,
		BlendIndices,
		ObjectIndex,
		Max
	};

	struct VertexElement
	{
		VertexElementType  type_{};
		VertexSemantic     semantic_{};
		unsigned char index_ = 0;
		bool per_instance_ = false;
		unsigned offset_ = 0;

		bool operator==(const VertexElement& o)
		{
			return type_ == o.type_ && semantic_ == o.semantic_ &&
				index_ == o.index_ && per_instance_ == o.per_instance_ &&
				offset_ == o.offset_;
		}

		bool operator!=(const VertexElement& o)
		{
			return type_ != o.type_ || semantic_ != o.semantic_ ||
				index_ != o.index_ || per_instance_ != o.per_instance_ ||
				offset_ != o.offset_;
		}
	};

	class VertexBuffer: public GPUObject
	{
	private:
		std::vector<unsigned char> cpu_data_;
		unsigned total_count_ = 0;
		unsigned per_size_ = 0;
		std::vector<VertexElement> elements_;
		unsigned long long element_hash_ = 0ULL;
		unsigned element_mask_ = 0;
		LockState lock_state_ = LockState::Hardware;
		unsigned lock_start_ = 0;
		unsigned lock_count_ = 0 ;
		void *lock_scratch_data_ = nullptr;
		bool dynamic_ = false ;
		bool shadowed_ = false ;
	public:
		virtual ~VertexBuffer() override;

		///override GPUObject
		virtual void OnDeviceLost() override;
		virtual void OnDeviceReset() override;
		virtual void Release() override;
		void Unlock();
		void UnmapBuffer();
	};
}