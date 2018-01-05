#pragma once

ccdk_namespace_mpl_start

/* if true, get type, otherwise get compile error */
template<bool B, typename T> struct enable_if {  };
template<typename T> struct enable_if<true,T> { typedef T type; };


/* if false, get type, otherwise get compile error */
template<bool B, typename T> struct disable_if {};
template<typename T> struct disable_if<false, T> { typedef T type; };

template<bool B,typename T> 
using enable_if_t = typename enable_if<B, T>::type;

template<bool B, typename T>
using disable_if_t = typename disable_if<B, T>::type;


ccdk_namespace_mpl_end