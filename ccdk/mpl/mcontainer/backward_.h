#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

/* push back */
template<typename Container, typename T> struct push_back_ : Container::template push_back<T> {};
template<typename Container, typename T> using push_back_t = typename push_back_<Container, T>::type;

/* pop back */
template<typename Container> struct pop_back_ { typedef typename Container::pop_back type; };
template<typename Container> using pop_back_t = typename Container::pop_back;

/* back */
template<typename Container> struct back_ { typedef typename Container::back type; };
template<typename Container> using back_t = typename Container::back;

ccdk_namespace_mpl_end