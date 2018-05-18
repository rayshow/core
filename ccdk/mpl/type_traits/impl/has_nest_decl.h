#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/declval.h>
#include<ccdk/mpl/base/logic_.h>

// test weather type T::nest_name is exists
#define CCDK_TT_HAS_NEST_TYPE_DECL(name, nest_name)								   \
namespace name##_impl                                                              \
{                                                                                  \
	template<typename T>                                                           \
	struct has_nest_ ## name ## _helper                                            \
	{                                                                              \
		template<typename U, typename = typename U:: nest_name>                    \
		constexpr static  bool sfinae(int) { return true; };                       \
		template<typename U>                                                       \
		constexpr static  bool  sfinae(...){ return false; };                      \
		static constexpr bool value = sfinae<T>(0);                                \
	};                                                                             \
}                                                                                  \
template<typename T> struct has_nest_ ## name:                                     \
	public bool_< name##_impl::has_nest_ ## name ## _helper<T>::value >{};         \
template<typename T>                                                               \
	static constexpr bool has_nest_ ## name ## _v = has_nest_ ## name<T>::value;

// test weather value T::nest_name is exists
#define CCDK_TT_HAS_NEST_CONSTEXPR_DECL(name, nest_name)                                             \
template<typename T, typename V>                                                             \
struct has_nest_##name                                                                       \
{                                                                                            \
	template<typename U, typename P, P p= U::nest_name>                                      \
	constexpr static  bool sfinae(int) { return is_base_same_v<P,decltype(U::nest_name)>; }; \
	template<typename U, typename P>                                                         \
	constexpr static  bool  sfinae(...) { return false; };                                   \
	static constexpr bool value = sfinae<T, V>(0);                                           \
};                                                                                           \
template<typename T, typename V>                                                             \
static constexpr bool has_inner_##name##_v = has_nest_##name<T, V>::value;