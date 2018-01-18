#pragma once

#include<exception>
#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_sp_start

class bad_weak_ptr : public std::exception
{
public:
	bad_weak_ptr() noexcept {}
	virtual const char* what() const noexcept override { return ("bad_weak_ptr"); }
};

ccdk_namespace_mpl_sp_end