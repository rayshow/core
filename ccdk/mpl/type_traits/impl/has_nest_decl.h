#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/declval.h>
#include<ccdk/mpl/base/logic_.h>

// test weather type T::nest_name is exists
#define CCDK_TT_HAS_NEST_TYPE_DECL(TraitName, NestName)							   \
namespace NestName##ns                                                             \
{                                                                                  \
	template<typename T>                                                           \
	struct TraitName ## _impl                                                      \
	{                                                                              \
		template<typename U, typename = typename U:: NestName>                     \
		constexpr static  bool sfinae(int) { return true; };                       \
		template<typename U>                                                       \
		constexpr static  bool sfinae(...){ return false; };                       \
		static constexpr bool value = sfinae<T>(0);                                \
	};                                                                             \
}                                                                                  \
template<typename T> struct TraitName:                                             \
	public bool_< NestName ## ns::TraitName ## _impl<T>::value >{};                \
template<typename T>                                                               \
	static constexpr bool TraitName ## _v = TraitName<T>::value;

// test weather value T::nest_name is exists
#define CCDK_TT_HAS_NEST_CONSTEXPR_DECL(TraitName, NestName)                                 \
template<typename T, typename V>                                                             \
struct TraitName                                                                             \
{                                                                                            \
	template<typename U, typename P, P p= U::NestName>                                       \
	constexpr static  bool sfinae(int) { return is_base_same_v<P,decltype(U::NestName)>; };  \
	template<typename U, typename P>                                                         \
	constexpr static  bool  sfinae(...) { return false; };                                   \
	static constexpr bool value = sfinae<T, V>(0);                                           \
};                                                                                           \
template<typename T, typename V>                                                             \
static constexpr bool TraitName##_v = TraitName##name<T, V>::value;