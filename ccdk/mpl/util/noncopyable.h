#pragma once

#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_util_start

	struct noncopyable
	{
		noncopyable() = default;

		noncopyable(const noncopyable&) = delete;

		noncopyable& operator=(const noncopyable&) = delete;
	};

ccdk_namespace_mpl_util_end