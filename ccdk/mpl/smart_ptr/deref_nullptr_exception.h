#pragma once

#include<exception>
#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_sp_start


struct deref_nullptr_exception
	:std::exception
{
	const char* msg;

	deref_nullptr_exception(const char* inMsg)
		:msg{ inMsg } {}

	virtual char const*
		what() const noexcept  override
	{
		return msg;
	}
};


ccdk_namespace_mpl_sp_end