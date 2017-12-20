#pragma once

#include<exception>
#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_fs_start

struct bad_cast_exception
	:std::exception
{
	const char* msg;

	bad_cast_exception(const char* inMsg)
		:msg{ inMsg } {}

	virtual char const*
		what() const noexcept  override
	{
		return msg;
	}
};

ccdk_namespace_mpl_fs_end