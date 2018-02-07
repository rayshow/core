#pragma once

#include<ccdk/compile.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>
#include<ccdk/mpl/mcontainer/iterator_.h>
#include<ccdk/mpl/mcontainer/algorithm/find_.h>
#include<ccdk/mpl/mcontainer/algorithm/lambda_.h>

ccdk_namespace_mpl_start

template<typename Begin, typename End, typename Pred>
struct filter_iterator_
{
	typedef iter_find_if_t<next_t<Begin>, End, Pred>   next_iter;
	typedef filter_iterator_<next_iter, End, Pred>     next;
	typedef deref_t<Begin>                             deref;
};

template<typename Container, typename Pred>
struct filter_view_
{
	typedef lambda_<Pred>                                               pred_type;
	typedef begin_t<Container>                                          container_begin;
	typedef end_t<Container>										    container_end;
	typedef iter_find_if_t<container_begin, container_end, pred_type>   first_iter;

	typedef filter_iterator_<first_iter, container_end, pred_type>      begin;
	typedef filter_iterator_<container_end, container_end, pred_type>   end;
};

ccdk_namespace_mpl_end