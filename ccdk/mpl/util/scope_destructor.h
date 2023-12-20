#pragma  once


#include<ccdk/mpl/mpl_module.h>
ccdk_namespace_mpl_util_start

class ScopeCallImpl
{
public:
	virtual ~ScopeCallImpl() {}
};

template<typename FN>
class ScopeCallObjectFnImpl : public ScopeCallImpl
{
private:
	FN const& Fn;
public:
	ScopeCallObjectFnImpl(FN const& InFn) :Fn{ InFn } {}
	~ScopeCallObjectFnImpl() { Fn(); }
};

static auto NULL_LAMBDA = []() {};
using NULL_LAMBDA_TYPE = decltype(NULL_LAMBDA);

class scope_destructor
{
private:
	ScopeCallImpl * Impl;
	char            StackMemory[sizeof(ScopeCallObjectFnImpl<NULL_LAMBDA_TYPE>)];
public:

	template<typename FN>
	scope_destructor(FN const& Fn) :Impl{ nullptr }
	{
		Impl = new(StackMemory) ScopeCallObjectFnImpl<FN>(Fn);
	}

	~scope_destructor()
	{
		if (Impl)
		{
			Impl->~ScopeCallImpl();
			Impl = nullptr;
		}
	}
};


ccdk_namespace_mpl_util_end
