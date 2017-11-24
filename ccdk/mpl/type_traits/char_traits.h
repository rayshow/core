#pragma once

namespace ccdk
{
	namespace mpl
	{
		template<typename Ch> struct char_traits {};

		template<>
		struct char_traits<char>
		{
			static constexpr char end = '\0';
		};

		template<>
		struct char_traits<wchar_t>
		{
			static constexpr char end = L'\0';
		};

		template<>
		struct char_traits<char16_t>
		{
			static constexpr char16_t end = u'\0';
		};

		template<>
		struct char_traits<char32_t>
		{
			static constexpr char16_t end = U'\0';
		};
	}
}