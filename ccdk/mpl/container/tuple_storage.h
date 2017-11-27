#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/container/val_pack.h>
#include<ccdk/mpl/container/ebo.h>

namespace ccdk
{
	namespace mpl
	{
		template<uint32 size, typename T, typename... Args>
		struct tuple_storage;

		template<uint32 size, uint32... indice, typename... Args>
		struct tuple_storage<size, indice_pack<indice...>, Args...> 
			:public iebo< indice, Args>...
		{
			typedef tuple_storage<size, indice_pack<indice...>, Args...> type;
			static constexpr uint32 L = size;

			constexpr tuple_storage() :iebo< indice, Args >{}... {}

			//for initalize from another tuple_storage with prefect forward
			template<typename T>
			constexpr tuple_storage(T&& t) 
				: iebo< indice, Args>( ebo_at<indice>(util::forward<T>(t)) )...
			{}

			//move
			constexpr tuple_storage(Args&&... args) 
				: iebo< indice, Args >(util::move(args))...
			{}

			//copy
			constexpr tuple_storage(Args const&... args) 
				: iebo< indice, Args >(args)...
			{}

			//indice move 
			template<uint32... indice1, typename... Args1>
			constexpr tuple_storage(indice_pack<indice1...>, Args1&&... args)
				: iebo< indice1, Args1 >(util::move(args))...
			{}
		};

	}
}