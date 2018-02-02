#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

/* front */
template<typename Container> struct front_ { typedef typename Container::front type; };
template<typename Container> using front_t = typename front_<Container>::type;

/* first of arg pack */
template<typename... Args> struct first_  {};
template<typename T, typename... Args> struct first_<T, Args...> { typedef T type; };
template<typename... Args> using first_t = typename first_<Args...>::type;

/* pop front */
template<typename Container> struct pop_front_ { typedef typename Container::pop_front type; };
template<typename Container> using pop_front_t = typename Container::pop_front;

/* pop front n */
template<typename Container, uint32 N> struct pop_front_n_ :pop_front_n_< pop_front_t<Container>, N - 1 > {};
template<typename Container> struct pop_front_n_< Container, 0> { typedef Container type; };
template<typename Container, uint32 N> using pop_front_n_t = typename pop_front_n_<Container, N>::type;

/* push front */
template<typename Container, typename T> struct push_front_: Container::template push_front<T> {};
template<typename Container, typename T> using push_front_t = typename push_front_<Container, T>::type;


ccdk_namespace_mpl_end