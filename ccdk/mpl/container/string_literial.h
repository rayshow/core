
#include<ccdk/type.h>
#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/base/uint_.h>
#include<ccdk/mpl/base/make_indice.h>
#include<ccdk/mpl/type_traits/char_traits.h>
#include<ccdk/mpl/type_traits/remove_dim.h>
#include<ccdk/mpl/type_traits/array_length.h>


namespace ccdk
{
	namespace mpl
	{
		//L length of string include '\0'
		template<typename Ch, uint32 L>
		struct string_literial
		{
			Ch storage[L];
			typedef Ch const* pointer;
			typedef string_literial<Ch, L> type;
			typedef const type const_type;

			
			template<uint32... args>
			constexpr string_literial(indice_pack<args...>, pointer arr) noexcept
				: storage{ arr[args]..., char_traits<Ch>::end }
			{}

			template<uint32... args1, uint32... args2, uint32... args3>
			constexpr string_literial(indice_pack<args1...>, indice_pack<args2...>,
				indice_pack<args3...>, pointer arr1, pointer arr2)
			{}

			template<uint32... args1, uint32... args2>
				constexpr string_literial(indice_pack<args1...>, indice_pack<args2...>,
					Ch c, pointer arr)
				:storage { arr[args1]..., c, arr[args2]...}
			{}

			constexpr string_literial(pointer arr) :
				string_literial(make_indice<L-1>{}, arr) {}


			template<typename T, T index>
			constexpr auto operator[](integer_<T,index>) const
			{
				return storage[index];
			}

			template<uint32 L2>
			constexpr bool operator==(const string_literial<Ch,L2>& l)
			{
				if (L != L2) return false;
				for (uint32 i = 0; i < L; ++i) if (storage[i] != l.storage[i]) return false;
				return true;
			}

			constexpr operator pointer() const
			{
				return &storage[0];
			}

			//find single char
			constexpr int find_first(Ch a) const
			{
				for (int i = 0; i < L; ++i) { if (a == storage[i]) return i; }
				return L;
			}

			constexpr int find_last(Ch a) const
			{
				for (int i = L - 1; i >= 0; --i) { if (a == storage[i]) return i; }
				return L;
			}

			template<uint32 start, uint32 end, typename = check_in_range2<start, end, 0, L> >
			constexpr auto substr() const
			{
				typedef string_literial<Ch, end - start + 1>{
					make_indice_from<start, end>{},
					storage
				};
			}

			template<uint32 index, typename = check_in_range<index , 0, L> >
			constexpr auto  replace(Ch c) const
			{
				return string_literial<Ch, L>{ 
					make_indice<index>{},
					make_indice_from<index + 1,L>{},
					c,
					storage 
				};
			}
		};

		//use to simulate constexpr string
		namespace literals
		{
			template<typename T>
			inline constexpr auto _literal(const T& arr)
			{
				return string_literial<remove_dim_t<T>, array_len<T>::value>{ arr };
			}
		}

	}
}