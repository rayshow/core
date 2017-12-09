#pragma once

namespace ccdk
{
	namespace mpl
	{
		namespace util
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
}