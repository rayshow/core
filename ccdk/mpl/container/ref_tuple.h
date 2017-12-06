#include<ccdk/type.h>
#include<ccdk/mpl/base/make_indice.h>
#include<ccdk/mpl/function/create.h>
namespace ccdk
{
	namespace mpl
	{
		//special use
		namespace container
		{
			template<uint32 K, typename V>
			struct ref_item_t
			{
				V& v;
				ref_item_t(V& inV)
					:v{ inV }
				{
					//DebugFunctionName();
				}
			};

			template<uint32 K, typename V>
			V& ref_at(const ref_item_t<K, V>& r)
			{
				return r.v;
			}


			template<typename I, typename... Args>
			struct ref_tuple_impl;

			template<uint32... indice, typename... Args>
			struct ref_tuple_impl<indice_pack<indice...>, Args...>
				:ref_item_t<indice, Args>...
			{
				ref_tuple_impl(Args&... args)
					:ref_item_t<indice, Args>( args )...
				{
					//DebugFunctionName();
				}
			};

			template<typename... Args>
			struct ref_tuple:
				ref_tuple_impl< make_indice<sizeof...(Args)>,Args...>
			{
				typedef ref_tuple_impl< make_indice<sizeof...(Args)>, Args...> base_type;
				ref_tuple(Args... args)
					:base_type{ args... }
				{
					//DebugFunctionName(); 
				}

				template<uint32 index>
				constexpr decltype(auto) at()
				{
					return ref_at<index>(*this);
				}
			};
		}

		constexpr fn_detail::create_raw<container::ref_tuple> create_ref_tuple{};
	}
}