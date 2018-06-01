#pragma once

#include<ccdk/io/io_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/impl/has_member_decl.h>
#include<ccdk/mpl/util/addressof.h>

ccdk_namespace_io_start

using namespace ccdk::mpl;

//sizeof<T> is real data size of T, no margin or pointer
template<typename T> struct is_real_size { enum { value = false }; };
template<> struct is_real_size<bool> { enum { value = true }; };
template<> struct is_real_size<float> { enum { value = true }; };
template<> struct is_real_size<double> { enum { value = true }; };
template<> struct is_real_size<uint8> { enum { value = true }; };
template<> struct is_real_size<uint16> { enum { value = true }; };
template<> struct is_real_size<uint32> { enum { value = true }; };
template<> struct is_real_size<uint64> { enum { value = true }; };
template<> struct is_real_size<int8> { enum { value = true }; };
template<> struct is_real_size<int16> { enum { value = true }; };
template<> struct is_real_size<int32> { enum { value = true }; };
template<> struct is_real_size<int64> { enum { value = true }; };
template<> struct is_real_size<achar> { enum { value = true }; };
template<> struct is_real_size<wchar> { enum { value = true }; };

enum class rewind_mode :uint8 {
	start,
	current,
	end,
};

//serialize write base class
class writer
{
private:
	ptr::size_t _pos = 0;
public:
	template<
		typename T,
		typename = check_t< is_real_size<T>>  >
		CCDK_FORCEINLINE writer& operator<<(const T& t) {
		if (sizeof(T) != write(reinterpret_cast<const void*>(util::addressof(t)), sizeof(T))) {
			ccdk_throw(std::out_of_range{ "write error size!" });
		}
		return *this;
	}

	template<
		typename T,
		typename = check_t< not_< is_real_size<T>>>,
		typename = check_t< is_serializible<T>> >
	CCDK_FORCEINLINE writer& operator<<(T const& t) {
		return t.write_to(*this);
	}

	CCDK_FORCEINLINE ptr::size_t position() const { return _pos; }
	CCDK_FORCEINLINE void move(int32 offset) { _pos += offset; }
	virtual ptr::size_t write(const void* data, ptr::size_t size) = 0;
	virtual void rewind(rewind_mode mode = rewind_mode::current, uint32 offset = 0) {}
	virtual void close() {};
	virtual bool is_opened() { return false; }
	virtual bool reached_end() { return false; }
	virtual ~writer() { _pos = 0; close(); }
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
		if (sizeof(T) != read(util::addressof(t), sizeof(T))) {
			ccdk_throw(std::out_of_range{ "read size error" });
		}
		return *this;
	}

	template<
		typename T,
		typename = check_t< not_<is_real_size<T>>>,
		typename = check_t< is_deserializible<T>> >
	CCDK_FORCEINLINE reader& operator>>(T& t) {
		return t.read_from(*this);
	}

	CCDK_FORCEINLINE ptr::size_t position() const { return _pos; }
	CCDK_FORCEINLINE void move(int32 offset) { _pos += offset; }
	virtual ptr::size_t read(void* data, ptr::size_t size) = 0;
	virtual void rewind(rewind_mode mode = rewind_mode::current, uint32 offset = 0) {}
	virtual void close() {};
	virtual bool is_opened() { return false; }
	virtual bool reached_end() { return false; }
	virtual ~reader() { _pos = 0; close(); }
};

//test weather bool write_to(writer&) is exists
CCDK_TT_HAS_MEMBER_WITH_RET_DECL(has_member_write_to, write_to, writer&)

//test weather bool read_from(reader&) is exists
CCDK_TT_HAS_MEMBER_WITH_RET_DECL(has_member_read_from, read_from, reader&)

//test weather T is serializible
template<typename T>
struct is_serializible : or_<
	is_real_size<T>, 
	has_member_write_to<T, writer&>> {};

template<typename T>
static constexpr bool is_serializible_v = is_serializible<T>::value;

//test weather T is deserializible
template<typename T>
struct is_deserializible : or_<
	is_real_size<T>,
	has_member_read_from<T, reader&>> {};

template<typename T>
static constexpr bool is_deserializible_v = is_deserializible<T>::value;

ccdk_namespace_io_end