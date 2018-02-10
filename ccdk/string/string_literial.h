#pragma once

#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/type_traits/remove_dim.h>
#include<ccdk/mpl/type_traits/array_length.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/string/string_module.h>
#include<ccdk/string/char_traits.h>

ccdk_namespace_string_start

using namespace mpl;

	/* Length length of string include '\0' */
	template< typename Char, uint32 Length>
	class string_literial
	{
	public:
		typedef Char												char_type;
		typedef Char												value_type;
		typedef uint32												size_type;
		typedef ptr::diff_t											different_type;
		typedef char_traits<Char>									traits_type;
		typedef string_literial										this_type;
		typedef Char*												pointer_type;
		typedef Char const*											const_pointer_type;
		typedef Char&												reference_type;
		typedef Char const&                                         const_reference_type;
		typedef it::iterator<Char*, string_literial>                iterator;
		typedef it::const_iterator<Char*, string_literial>          const_iterator;
		typedef it::reverse_iterator<Char*, string_literial>        reverse_iterator;
		typedef it::const_reverse_iterator<Char*, string_literial>  const_reverse_iterator;

		static constexpr uint32   npos = uint32(-1);
		static constexpr uint32   max_pos = uint32(-2);

		template<typename Char2, uint32 Length2>
		friend class string_literial;

	private:
		Char storage[Length];
		
	public:

		/*  constructor */
		template<uint32... indice>
		CCDK_FORCEINLINE constexpr string_literial( indice_pack<indice...>, const_pointer_type pointer) noexcept : storage{ pointer[indice]..., Char(0) } {}

		/* merge 3-part */
		template< uint32... indice1, uint32... indice2, uint32... indice3 >
		CCDK_FORCEINLINE constexpr string_literial( indice_pack<indice1...>, indice_pack<indice2...>, indice_pack<indice3...>, const_pointer_type pointer1, const_pointer_type pointer2) {}

		/* insert middle char */
		template< uint32... indice1, uint32... indice2>
		CCDK_FORCEINLINE constexpr string_literial( indice_pack<indice1...>, indice_pack<indice2...>, char_type c, const_pointer_type pointer)
			:storage { pointer[indice1]..., c, pointer[indice2]... } {}

		/* copy */
		CCDK_FORCEINLINE constexpr string_literial(const_pointer_type pointer) : string_literial(make_indice<Length-1>{}, pointer) {}

		/* comptile and runtime index */
		template< typename T, T index > CCDK_FORCEINLINE Char& operator[](uint32 index) { return storage[index]; }
		template< typename T, T index > CCDK_FORCEINLINE Char const& operator[](uint32 index) { return storage[index]; }
		template< typename T, T index > CCDK_FORCEINLINE constexpr Char& operator[](compile_t<T,index>) { return storage[index]; }
		template< typename T, T index > CCDK_FORCEINLINE constexpr Char const& operator[](compile_t<T, index>) const { return storage[index]; }

		template<typename Char2, uint32 Length2>
		CCDK_FORCEINLINE constexpr bool operator==(const string_literial<Char2,Length2>& other)
		{
			if (Length != Length2) return false;
			for (uint32 i = 0; i < Length; ++i) if (storage[i] != other.storage[i]) return false;
			return true;
		}

		CCDK_FORCEINLINE constexpr operator const_pointer_type() const { return &storage[0]; }

		/* find first single char */
		CCDK_FORCEINLINE constexpr int find_first(Char c) const { for (int i = 0; i < Length; ++i) { if (c == storage[i]) return i; } return Length; }

		/* find last single char */
		CCDK_FORCEINLINE constexpr int find_last(Char a) const { for (int i = Length - 1; i >= 0; --i)  { if (a == storage[i]) return i; } return Length; }

		/* sub string */
		template< uint32 start, uint32 end, typename = check_in_range2<start, end, 0, Length> >
		CCDK_FORCEINLINE constexpr auto substr() const { return string_literial<Char, end - start + 1>{ make_indice_from<start, end>{}, storage }; }

		/* runtime replace */
		template< uint32 index, typename = check_in_range<index , 0, Length> >
		CCDK_FORCEINLINE constexpr auto replace(Char c) const { return string_literial<Char, Length>{  make_indice<index>{}, make_indice_from<index + 1,Length>{}, c, storage  }; }
	};

	template<typename T>
	inline constexpr auto _literal(const T& arr)
	{
		return string_literial<remove_dim_t<T>, array_len<T>::value>{ arr };
	}

ccdk_namespace_string_end