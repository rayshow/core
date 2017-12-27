#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/is_base_of.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/util/noncopyable.h>
#include<ccdk/mpl/fusion/compress_pair.h>
#include<ccdk/mpl/smart_ptr/default_deleter.h>

ccdk_namespace_mpl_sp_start

//resource smart ptr

struct resource_base
{
	virtual void* pointer() const noexcept = 0;
	virtual void* release() noexcept = 0;
	virtual resource_base* clone() const = 0;
	virtual ~resource_base() {  }
};

//use default_deleter
template<typename Type>
struct default_resource_base : public resource_base
{
	typedef default_deleter<Type> deleter_type;
	typedef Type* pointer_type;
private:
	fs::compress_pair<deleter_type, pointer_type> pair;
public:

	/* copy */
	CCDK_FORCEINLINE default_resource_base(const default_resource_base& other) : pair{ other.pair } {}

	/* move */
	CCDK_FORCEINLINE default_resource_base(default_resource_base&& other) : pair{ util::move(other.pair) } {}

	/* pointer initalize */
	CCDK_FORCEINLINE default_resource_base(Type* ptr) : pair{ ptr } {}

	/* get managed pointer */
	virtual void* pointer() const noexcept override  { return (void*)pair.second; }

	/*  */
	virtual resource_base* clone() const override { return new default_resource_base{ *this }; }

	/* unhold pointer */
	virtual void* release() noexcept override  { pointer_type ret = pair.second; pair.second = nullptr; return ret; }

	//destroy managed object
	virtual ~default_resource_base() { pair.get_first()(pair.second); DebugValue(">> default resource base destructor"); }
};

//special deleter
template<typename Type, typename Deleter>
struct deleter_resource_base :public resource_base
{
	typedef Deleter deleter_type;
	typedef Type*   pointer_type;
private:
	fs::compress_pair<Deleter, pointer_type> pair;

public:

	//copy
	CCDK_FORCEINLINE deleter_resource_base(const deleter_resource_base& other) : pair{ other.pair } {}

	//move
	CCDK_FORCEINLINE deleter_resource_base(deleter_resource_base&& other) : pair{ util::move(other.pair) } {}

	//
	CCDK_FORCEINLINE deleter_resource_base(Type* ptr, const Deleter& dl) : pair{ dl, ptr } {}


	virtual void* pointer() const noexcept override  { return (void*)pair.second; }

	virtual void* release() noexcept override { pointer_type ret = pair.second; pair.second = nullptr; return ret; }

	//clone self
	virtual resource_base* clone() const override { return new deleter_resource_base{ *this }; }


	//delete resource
	virtual ~deleter_resource_base() { pair.get_first()(pair.second); DebugValue(">> default resource base destructor"); }
};


ccdk_namespace_mpl_sp_end