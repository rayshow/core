#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/mcontainer/algoritm/merge_.h>
#include<ccdk/mpl/mcontainer/algoritm/make_val_pack.h>

ccdk_namespace_mpl_start

template<uint32 End>
using make_indice = typename make_val_pack<uint32, 0, End>::type;

template<uint32 Start, uint32 End>
using make_indice_from = typename make_val_pack< uint32, Start, End>::type;

template<uint32 End, uint32 IgnoreStart, uint32 IgnoreEnd>
using make_indice_ignore = typename merge_< make_indice<IgnoreStart>, make_indice_from<IgnoreEnd, End>>::type;

ccdk_namespace_mpl_end