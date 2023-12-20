#pragma once

#include "type.h"
#include<stdio.h>

namespace Luapp
{
	struct BinaryHeader
	{
		uint8  signature[4];
		uint8  version;
		uint8  format;
		uint8  luacData[6];
		uint8  cintSize;
		uint8  sizetSize;
		uint8  instructionSize;
		uint8  integerSize;
		uint8  numberSize;
		uint64 luacInt;
		double luacNumber;
	};

	constexpr BinaryHeader HEADER = {
		{'\x1b','L','u','a'}
		, 0x53
		, 0
		, {'\x19', '\0x93', '\r', '\n', '\x1a', '\n'}
		, 4
		, 8
		, 4
		, 8
		, 0x5678
		, 370.5
	};

	enum class EConstantTag
	{
		Nil = 0x00,
		Boolean = 0x01,
		Number = 0x03,
		Integer = 0x13,
		ShortString = 0x04,
		LongString = 0x14,
	};

	struct Constant
	{

	};

	struct UpValue
	{
		uint8 inStack;
		uint8 index;
	};

	struct LocVar
	{

	};

	struct ProtoType
	{
		char*  source;
		uint32 lineDefined;
		uint32 lastLineDefined;
		uint8  nParams;
		bool   isvarg;
		uint8  maxStackSize;
		int32  nUpValues;
		int32  nConstants;
		int32  nSizeCodes;
		int32* Code;
		Constant* constants;
		UpValue*  upvalues;
		ProtoType* childrenProtoes;
		int32*  lineInfos;
		LocVar* locVars;
		char**  upValueNames;
	};

	struct BinaryChunk
	{
		BinaryHeader header;

	};

	class BinaryReader
	{
	private:
		uint8 * bytes;
		uint32  pos;
	public:
		BinaryReader() : bytes{ nullptr }, pos{ 0 } {}
		BinaryReader(uint8* InBytes, uint32 InPos) : bytes{ InBytes }, pos{ InPos } {}

		template<typename T> T Read();
	};
}

