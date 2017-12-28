
#pragma once
#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/smart_ptr/smart_ptr_fwd.h>
#include<ccdk/mpl/smart_ptr/default_deleter.h>
#include<ccdk/mpl/smart_ptr/default_ref_count.h>

ccdk_namespace_mpl_sp_start

template<typename T,
	typename D = default_deleter<T> , 
	typename R = default_ref_count<> >
class enable_share
{
private:
	share_ptr<T, D, R>  this_share;  /* keep a ref of this */
public:

	enable_share()
	{
	}

	share_ptr<T, D, R> share_from_this() const noexcept
	{
		return this_share;
	}
};


ccdk_namespace_mpl_sp_end