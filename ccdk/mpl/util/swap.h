#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/compatible_type.h>
#include<ccdk/mpl/type_traits/is_compatible.h>
#include<ccdk/mpl/type_traits/is_convertible.h>
#include<ccdk/mpl/type_traits/remove_pointer.h>
#include<ccdk/mpl/type_traits/has_constructor.h>
#include<ccdk/mpl/type_traits/has_assigner.h>
#include<ccdk/mpl/type_traits/is_fundamental.h>
#include<ccdk/mpl/type_traits/is_compound.h>
#include<ccdk/mpl/type_traits/is_class.h>
#include<ccdk/mpl/util/move.h>

ccdk_namespace_mpl_util_start
	
//template< typename T1, typename T2>
//CCDK_FORCEINLINE void swap(T1& t1, T2& t2)
//{
//	DebugFunctionName();
//}

#if defined( CCDK_COMPILER_MSVC ) 

	//no suitable implements found, for msvc-17+ to  to get detail line and file error place
	template< typename T1, typename T2,  typename = check_t<false_> >
	CCDK_FORCEINLINE void swap(T1& t1, T2& t2) {}


#elif defined( CCDK_COMPILER_GCC )
	
	//no suitable implements found, for gcc  to get detail line and file error place 
	template< typename T1, typename T2>
	CCDK_FORCEINLINE void swap(T1& t1, T2& t2);

#else //clang or some compiler not found suitable method

	template< typename T1, typename T2 >
	CCDK_FORCEINLINE void swap(T1& t1, T2& t2)
	{
		static_assert(false_::value, "no suitable swap found");
	}
	
#endif

	//for same type pointer
	template<typename T>
	CCDK_FORCEINLINE void swap(T*& t1, T*& t2) noexcept
	{
		DebugValue("same pointer swap");
		T* tmp = t1;
		t1 = t2;
		t2 = tmp;
	}

	

	//for different type pointer, must be compatible
	template<typename T1, typename T2, typename = check_t< is_compatible<T1*, T2*> >  >
	CCDK_FORCEINLINE void swap(T1*& t1, T2*& t2) noexcept
	{
		DebugValue("different pointer swap");
		T1* tmp = t1;
		t1 = (T1*)t2;
		t2 = (T2*)tmp;
	}

	//for array
	template< typename T, ptr::size_t N >
	CCDK_FORCEINLINE void swap(T(&t1)[N], T(&t2)[N])
	{
		DebugValue("array swap");
		for (int i = 0; i < N; ++i)
		{
			//adl select
			swap(t1[i], t2[i]);
		}
	}

	//suitable compound type with nothrow move constructor and move assigner class
	template<typename T,
		typename = check_t< is_class<T>>,
		typename = check_t< has_move_assigner<T> >,
		typename = check_t< has_move_constructor<T> >
	>
	CCDK_FORCEINLINE void swap(T& t1, T& t2)
	{
		DebugValue("class move swap");
		T tmp{ util::move(t1) };
		t1 = util::move(t2);
		t2 = util::move(tmp);
	}

	//suit for fundamental type 
	template<typename T, typename = check_t< is_fundamental<T> >>
	CCDK_FORCEINLINE void swap(T& t1, T& t2)
	{
		DebugValue("fundamental swap");
		T tmp = t1;
		t1 = t2;
		t2 = tmp;
	}

ccdk_namespace_mpl_util_end


namespace test_swap
{
		//class with swap specific
		struct test_swap_t
		{
			int value;
			test_swap_t(int v) :value{ v } {}
			test_swap_t() {}
			void swap(test_swap_t& other)  { using  ccdk::mpl::util::swap; swap(value, other.value); }
		};

		//adl swap test
		CCDK_FORCEINLINE void swap(test_swap_t& t1, test_swap_t& t2)
		{
			DebugFunctionName();
			t1.swap(t2);
		}

		//class with move
		struct test_swap_t2
		{
			int value;
			test_swap_t2(int v) :value{ v } {}
			test_swap_t2(test_swap_t2&& other)  noexcept:value{ other.value } { ccdk::DebugValue("move"); }
			test_swap_t2& operator=(test_swap_t2&& other) noexcept { value = other.value; ccdk::DebugValue("move assign"); return *this; }
		};
}

