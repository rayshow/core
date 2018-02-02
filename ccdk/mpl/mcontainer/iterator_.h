#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

/* begin */
template<typename Container> struct begin_ { typedef typename Container::begin type; };
template<typename Container> using begin_t = Container::begin;

/* end */
template<typename Container> struct end_ { typedef typename Container::end type; };
template<typename Container>  using end_t = Container::end;

/* rbegin */
template<typename Container> struct rbegin_{ typedef typename Container::rbegin type; };
template<typename Container> using rbegin_t = Container::rbegin;

/* rend */
template<typename Container> struct rend_ { typedef typename Container::rend type; };
template<typename Container> using rend_t = Container::rend;

/* deref */
template<typename It> struct deref_ { typedef typename It::deref type; };
template<typename It> using deref_t = typename It::deref;

/* next */
template<typename It> struct next_ { typedef typename It::next type; };
template<typename It> using next_t = typename It::next;

ccdk_namespace_mpl_end