#pragma once

#include<exception>
#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_sp_start


struct out_of_range_exception
	:std::exception
{
	const char* msg;

	out_of_range_exception(const char* inMsg)
		:msg{ inMsg } {}

	virtual char const*
		what() const noexcept  override
	{
		return msg;
	}
};


ccdk_namespace_mpl_sp_end