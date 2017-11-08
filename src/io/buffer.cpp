
#include"buffer.h"
#include"binary_file.h"
#include"logger.h"


namespace Aurora3D
{
	size_type RWBuffer::Write(const uint8* data, size_type size)
	{
		A3D_ASSERT(GetWritedSize() <= buffer_size);
		uint32 free = buffer_size - GetWritedSize();
		if (A3D_UNLIKELY(free < size))
		{
			A3D_WARNING("buffer nearly fulled!");
			return 0;
		}
		memcpy(buffer, data, size);
		AddWriteSize(size);
		return size;
	}

	MappingBuffer::MappingBuffer(const std::string& file_name)
	{
		
	}

	
}