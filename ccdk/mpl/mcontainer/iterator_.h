#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

/* begin */
template<typename Container> struct begin_ { typedef typename Container::begin type; };
template<typename Container> using begin_t = typename Container::begin;

/* end */
template<typename Container> struct end_ { typedef typename Container::end type; };
template<typename Container>  using end_t = typename Container::end;

/* rbegin */
template<typename Container> struct rbegin_{ typedef typename Container::rbegin type; };
template<typename Container> using  rbegin_t = typename Container::rbegin;

/* rend */
template<typename Container> struct rend_ { typedef typename Container::rend type; };
template<typename Container> using rend_t = typename Container::rend;

/* deref */
template<typename It> struct deref_ { typedef typename It::deref type; };
template<typename It> using deref_t = typename It::deref;

/* next */
template<typename It> struct next_ { typedef typename It::next type; };
template<typename It> using next_t = typename It::next;

template<typename It> struct prior_ { typedef typename It::prior type; };
template<typename It> using prior_t = typename It::prior;

/* forward N step */
template<typename It, int32 N, bool = (N>0) > struct forward_ :forward_< next_t<It>, N - 1> {};
template<typename It, int32 N> struct forward_<It,N,false> :forward_< prior_t<It>, N - 1> {};
template<typename It> struct forward_< It, 0> { typedef It type; };
template<typename It, int32 N> using forward_t = typename forward_< It, N>::type;

ccdk_namespace_mpl_end