#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/mcontainer/merge.h>
#include<ccdk/mpl/mcontainer/make_val_pack.h>

ccdk_namespace_mpl_start

		template<uint32 end>
		using make_indice = typename make_val_pack<uint32, 0, end>::type;

		template<uint32 start, uint32 end>
		using make_indice_from = typename make_val_pack< uint32, start, end>::type;

		template<uint32 end, uint32 ignore_start, uint32 ignore_end>
		using make_indice_ignore = typename merge< make_indice<ignore_start>, make_indice_from<ignore_end, end>>::type;

ccdk_namespace_mpl_end