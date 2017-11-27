#pragma once
#include<ccdk/type.h>
#include<cassert>

namespace ccdk
{
	namespace mpl
	{
		struct string_view
		{
			const char *ptr;

			constexpr string_view(const char* inPtr) :ptr(inPtr) {}

			constexpr ptr::size_t find(char c) const
			{
				const char* s = ptr;
				for (; *s != '\0'; ++s);
				return s - ptr;
			}

		};
	}
}