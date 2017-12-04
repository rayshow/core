#pragma once

namespace ccdk
{
	namespace mpl
	{
		template<typename T>
		struct noncopyable
		{
			noncopyable() = default;

			noncopyable(const noncopyable&) = delete;

			noncopyable& operator=(const noncopyable&) = delete;
		};
	}
}