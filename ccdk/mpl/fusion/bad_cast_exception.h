#pragma once

#include<exception>
#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_fs_start

struct bad_any_cast :public std::exception
{
	bad_any_cast() {}

	virtual char const* what() const noexcept  override
	{
		return "bad any cast";
	}
};

struct bad_verient_cast :public std::exception
{
	bad_verient_cast() {}

	virtual char const* what() const noexcept  override
	{
		return "bad verient cast";
	}
};

ccdk_namespace_mpl_fs_end