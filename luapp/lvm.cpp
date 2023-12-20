#include "lvm.h"
#include<string>

namespace Luapp
{
	template<>
	uint8 BinaryReader::Read<uint8>()
	{
		return static_cast<uint8>(bytes[pos++]); 
	}

	template<>
	char BinaryReader::Read<char>()
	{
		return static_cast<char>(bytes[pos++]);
	}

	template<>
	uint16 BinaryReader::Read<uint16>()
	{
		uint16 ret = *reinterpret_cast<uint16*>( (bytes + pos) );
		pos += sizeof(uint16);
		return ret;
	}

	template<>
	int16 BinaryReader::Read<int16>()
	{
		int16 ret = *reinterpret_cast<int16*>((bytes + pos));
		pos += sizeof(int16);
		return ret;
	}

	template<>
	uint32 BinaryReader::Read<uint32>()
	{
		uint32 ret = *reinterpret_cast<uint32*>((bytes + pos));
		pos += sizeof(uint32);
		return ret;
	}

	template<>
	int32 BinaryReader::Read<int32>()
	{
		int32 ret = *reinterpret_cast<int32*>((bytes + pos));
		pos += sizeof(int32);
		return ret;
	}

	template<>
	uint64 BinaryReader::Read<uint64>()
	{
		uint64 ret = *reinterpret_cast<uint64*>((bytes + pos));
		pos += sizeof(uint64);
		return ret;
	}

	template<>
	int64 BinaryReader::Read<int64>()
	{
		int64 ret = *reinterpret_cast<int64*>((bytes + pos));
		pos += sizeof(int64);
		return ret;
	}

	template<>
	char* BinaryReader::Read<char*>()
	{
		uint64 size = Read<uint8>();
		if (size == 0) {
			return nullptr;
		}
		else if (size == 0xff) {
			size = Read<uint64>();
		}
		char* start = reinterpret_cast<char*>(bytes);
		char* ret = new char[size + 1];
		ret[size] = 0;
		if (ret == nullptr) return nullptr;
		memcpy(ret, start, size);
		bytes += size;
		pos += sizeof(int64);
		return ret;
	}

}