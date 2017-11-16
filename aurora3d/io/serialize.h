#pragma once
#include"serialize_trait.h"

namespace Aurora3D
{
	//serialize class
	class Writer
	{
	private:
		size_type writed_size = 0;
	public:
		template<typename T>  inline
			std::enable_if_t< IsSerializable<T>::value, Writer&>
				operator<<(const T& t);
		size_type GetWritedSize() const { return writed_size; }
		void AddWriteSize(uint32 size) { writed_size += size; }
		virtual size_type Write(const uint8* data, size_type size) = 0;
		virtual bool WritedEnd() = 0;
		virtual ~Writer() { writed_size = 0; }
	};

	template<typename T> inline
	std::enable_if_t< IsSerializable<T>::value, Writer&>
	 Writer::operator<<(const T& t)
	{
		this->Write((const uint8*)&t, sizeof(T));
		return *this;
	}

	template<>
	inline Writer& Writer::operator<<<AString>(const AString& t)
	{
		uint64 len = t.length();
		this->Write((const uint8*)&len, sizeof(uint64));
		this->Write((const uint8*)t.c_str(), t.length());
		return *this;
	}

	template<>
	inline Writer& Writer::operator<<<WString>(const WString& t)
	{
		assert(!t.empty());
		uint64 len = t.length();
		this->Write((const uint8*)&len, sizeof(uint64));
		this->Write((const uint8*)t.c_str(), t.length()*sizeof(wchar));
		return *this;
	}


	///deserialize class
	class Reader
	{
	protected:
		size_type  read_pos = 0;
	public:
		Reader() = default;
		template<typename T> 
			std::enable_if_t< IsSerializable<T>::value, Reader&>
				operator>>(T& type);
		size_type GetReadPos() const { return read_pos; }
		virtual bool ReadEnd() = 0;
		virtual size_type Read(uint8* buf, size_type size) = 0;
		virtual bool SkipFromBegin(size_type skip) = 0;
		virtual bool SkipFromCurrent(size_type skip) = 0;
		virtual bool SkipFromEnd(size_type skip) = 0;
		virtual ~Reader() { read_pos = 0; }
	};

	template<typename T> inline
	std::enable_if_t< IsSerializable<T>::value, Reader&>
	Reader::operator>>(T& type)
	{
		this->Read((uint8*)&type, sizeof(T));
		return *this;
	}

	template<>
	inline Reader& Reader::operator>><WString>(WString& str)
	{
		uint64 len = 0;
		this->Read((uint8*)&len, sizeof(uint64));
		AString read(len, ' ');
		this->Read((uint8*)read[0], sizeof(wchar)*len);
		return *this;
	}

	template<>
	inline Reader& Reader::operator>><AString>(AString& str)
	{
		int64 len = 0;
		this->Read((uint8*)&len, sizeof(uint64));
		AString read(len, ' ');
		this->Read((uint8*)&read[0], len*sizeof(char));
		str.swap(read);
		return *this;
	}
}