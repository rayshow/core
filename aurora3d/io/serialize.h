#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/impl/has_member_decl.h>
#include<ccdk/mpl/util/addressof.h>
#include<module.h>

a3d_namespace_io_start

using namespace ccdk::mpl;

//sizeof<T> is real data size of T, no margin or pointer
template<typename T> struct is_real_size { enum { value = false }; };
template<> struct is_real_size<bool>  { enum { value = true }; };
template<> struct is_real_size<float> { enum { value = true }; };
template<> struct is_real_size<double> { enum { value = true }; };
template<> struct is_real_size<uint8> { enum { value = true }; };
template<> struct is_real_size<uint16> { enum { value = true }; };
template<> struct is_real_size<uint32> { enum { value = true }; };
template<> struct is_real_size<uint64> { enum { value = true }; };
template<> struct is_real_size<int8>  { enum { value = true }; };
template<> struct is_real_size<int16> { enum { value = true }; };
template<> struct is_real_size<int32> { enum { value = true }; };
template<> struct is_real_size<int64> { enum { value = true }; };
template<> struct is_real_size<achar> { enum { value = true }; };
template<> struct is_real_size<wchar> { enum { value = true }; };

//test weather void write_to(writer&) is exists
CCDK_TT_HAS_MEMBER_WITH_RET_DECL(is_serialize_writable, write_to, void)

//test weather void read_from(reader&) is exists
CCDK_TT_HAS_MEMBER_WITH_RET_DECL(is_serialize_readable, read_from, void)

//serialize write base class
class writer
{
public:
	enum Mode :uint8 {
		Replace = 0,
		Append
	};

private:
	ptr::size_t _pos = 0;
public:
	template<
		typename T,
		typename = check_t< is_real_size<T>>  > 
	A3D_FORCEINLINE writer& operator<<(const T& t){
		ccdk_assert( sizeof(T) == 
			write(reinterpret_cast<const uint8*>(util::addressof(t)), sizeof(T)));
	}

	template<
		typename T,
		typename = check_t< not_< is_real_size<T>>>,
		typename = check_t< is_serialize_writable<T>> >
	A3D_FORCEINLINE writer& operator<<(T const& t) { t.write_to(*this); }

	A3D_FORCEINLINE ptr::size_t pos() const { return _pos; }
	A3D_FORCEINLINE void inc_pos(uint32 size) { _pos += size; }
	virtual ptr::size_t write(const uint8* data, ptr::size_t size) = 0;
	virtual bool close() {};
	virtual bool is_opened() = 0;
	virtual ~writer() { _pos = 0; }
};

//deserialize class
class reader
{
private:
	ptr::size_t  _pos = 0;
public:
	template<
		typename T,
		typename = check_t< is_real_size<T>> >  
	CCDK_FORCEINLINE reader& operator>>(T& t) {
		a3d_assert(sizeof(T) == read(&t, sizeof(T)));
	}

	template<
		typename T,
		typename = check_t< is_real_size<T>> >
		CCDK_FORCEINLINE reader& operator>>(T& t) {
		a3d_assert(sizeof(T) == read(&t, sizeof(T)));
	}

	ptr::size_t pos() const { return _pos; }
	virtual bool is_end() = 0;
	virtual bool is_opened() = 0;
	virtual ptr::size_t read(uint8* buf, ptr::size_t n) = 0;
	virtual bool skip_from_begin(ptr::size_t n) = 0;
	virtual bool skip_from_cur(ptr::size_t n) = 0;
	virtual bool skip_from_end(ptr::size_t n) = 0;
	virtual ~reader() { _pos = 0; }
};

a3d_namespace_io_end