#pragma once

#include<core/mpl/container/vector_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename S>
		struct VectorFront
		{
			static_assert(S::length != 0, "Vector length is zero.");
			typename typename S::t0 type;
		};

		template<typename S, typename T> 
		struct VectorPushFront 
		{
			static_assert(S::length <= A3D_MPL_VECTOR_CAPACITY, "Vector capacity is full, please increase A3D_MPL_VECTOR_CAPACITY.");
		};

		template<typename T, typenameprefix,... Args>
		struct VectorPushFront<Vector_<Argsprefix,...>, T>
		{
			typedef Vector_<T, Argsprefix,...> type;
		};

		template<typename S> struct VectorPopFront {};

		namespace detail
		{
			template<typename T0, typenameprefix,... TArgs>
			struct VectorPopFrontImpl
			{
				typedef Vector_<TArgsprefix,...> type;
			};
		}

		template<typenameprefix,... Args>
		struct VectorPopFront<Vector_<Argsprefix,...>>
			:public detail::VectorPopFrontImpl<Argsprefix,...>
		{
			static_assert(sizeofprefix,...(Args) >= 1, "Vector is empty.");
		};

	}
}