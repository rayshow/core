#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/base/val_pack.h>
#include<ccdk/mpl/base/val_pack_create.h>
#include<ccdk/mpl/base/val_pack_split.h>



namespace ccdk
{
	namespace mpl
	{
		template<uint32 end>
		using make_indice = typename val_pack_create<uint32, 0, end>::type;

		template<uint32 start, uint32 end, uint32... args>
		using make_indice_from = typename val_pack_create<uint32, start, end>::type;

		template<uint32 length, uint32 start, uint32 end>
		using make_indice_ingore = typename val_pack_split< uint32, start, end, make_indice<length> >::type;
	}
}