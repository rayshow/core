
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/declval.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/mpl_module.h>

#define CCDK_TT_HAS_ATTRIBUTE_DECL1(name )                                  \
namespace tt_impl                                                           \
{	template<typename T> struct has_attribute_##name##_impl{                \
		template<typename P, typename = decltype(makeval<P>().name)>        \
		static constexpr bool sfinae(int) { return true; }                  \
		template<typename P>                                                \
		static constexpr bool sfinae(...) { return false; }                 \
		constexpr static bool value = sfinae<T>(0); 	};                  \
	template<typename T, typename Ret>                                      \
struct name ## _is_type :is_same< Ret, decltype(makeval<T>().name)> {}; }   \
template<typename T> struct has_attribute_## name                           \
	:and_< bool_<                                                           \
		tt_impl::has_attribute_##name##_impl<T>::value >,                   \
		tt_impl::name ## _is_type<T,T*> >{};                                \
template<typename T>                                                        \
static constexpr bool has_attribute_## name ##_v = has_attribute_## name <T>::value;    \
template<typename T>                                                                    \
constexpr typename has_attribute_## name <T>::type has_attribute_## name ##_c{};        
