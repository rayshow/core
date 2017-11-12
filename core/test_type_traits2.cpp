#include<cstdio>
#include<utility>
#include<memory>
#include<string>
#include<typeinfo>

#include<core/mpl/base/bool_.h>
#include<core/mpl/base/and_.h>
#include<core/mpl/base/or_.h>


#include<core/mpl/type_traits/add_const.h>
#include<core/mpl/type_traits/add_const.h>
#include<core/mpl/type_traits/add_volatile.h>
#include<core/mpl/type_traits/add_cv.h>
#include<core/mpl/type_traits/add_lref.h>
#include<core/mpl/type_traits/add_rref.h>
#include<core/mpl/type_traits/add_top_const.h>
#include<core/mpl/type_traits/add_signed.h>
#include<core/mpl/type_traits/add_unsigned.h>
#include<core/mpl/type_traits/add_pointer.h>
#include<core/mpl/type_traits/decay.h>
#include<core/mpl/type_traits/align_of.h>
#include<core/mpl/type_traits/array_dim.h>
#include<core/mpl/type_traits/array_length.h>
#include<core/mpl/type_traits/remove_all_dim.h>
#include<core/mpl/type_traits/remove_dim.h>
#include<core/mpl/type_traits/remove_const.h>
#include<core/mpl/type_traits/remove_cv.h>
#include<core/mpl/type_traits/remove_pointer.h>
#include<core/mpl/type_traits/remove_ref.h>
#include<core/mpl/type_traits/remove_top_const.h>
#include<core/mpl/type_traits/remove_volatile.h>


#include<core/mpl/type_traits/is_void.h>
#include<core/mpl/type_traits/is_same.h>
#include<core/mpl/type_traits/is_unsigned.h>
#include<core/mpl/type_traits/is_signed.h>
#include<core/mpl/type_traits/is_union.h>
#include<core/mpl/type_traits/is_trivial.h>
#include<core/mpl/type_traits/is_stand_layout.h>
#include<core/mpl/type_traits/is_scalar.h>
#include<core/mpl/type_traits/is_rref.h>
#include<core/mpl/type_traits/is_lref.h>
#include<core/mpl/type_traits/is_ref.h>
#include<core/mpl/type_traits/is_polymorphic.h>
#include<core/mpl/type_traits/is_pointer.h>
#include<core/mpl/type_traits/is_pod.h>
#include<core/mpl/type_traits/is_object.h>
#include<core/mpl/type_traits/is_nullptr_t.h>
#include<core/mpl/type_traits/is_mobj_ptr.h>
#include<core/mpl/type_traits/is_mfn_ptr.h>
#include<core/mpl/type_traits/is_member_ptr.h>
#include<core/mpl/type_traits/is_integer.h>
#include<core/mpl/type_traits/is_fundamental.h>
#include<core/mpl/type_traits/is_function.h>
#include<core/mpl/type_traits/is_float.h>
#include<core/mpl/type_traits/is_final.h>
#include<core/mpl/type_traits/is_enum.h>
#include<core/mpl/type_traits/is_empty.h>
#include<core/mpl/type_traits/is_scalar.h>
#include<core/mpl/type_traits/is_convertible.h>
#include<core/mpl/type_traits/is_const.h>
#include<core/mpl/type_traits/is_compound.h>
#include<core/mpl/type_traits/is_class.h>
#include<core/mpl/type_traits/is_base_same.h>
#include<core/mpl/type_traits/is_base_of.h>
#include<core/mpl/type_traits/is_array.h>
#include<core/mpl/type_traits/is_arithmetic.h>
#include<core/mpl/type_traits/is_abstract.h>
#include<core/mpl/type_traits/is_instance_of.h>
//#include<core/mpl/type_traits/is_compatible.h>

#define AssertFalse(v) static_assert(!v, "")
#define AssertTrue(v) static_assert( v, "")
typedef int int2[2];

using namespace core::mpl;

int main()
{
	//bool wrap
	AssertTrue(true_::value);
	AssertTrue(true_c);
	AssertFalse(false_::value);
	AssertFalse(false_c);

	//is_same
	AssertTrue((is_same_v<int, int>));
	AssertFalse((is_same_v<int, int const>));
	AssertFalse((is_same_v<int, int volatile>));
	AssertFalse((is_same_v<int, int const volatile>));
	AssertFalse((is_same_v<int, char>));

	//is_void
	AssertTrue(is_void_v<void>);
	AssertTrue(is_void_v<void const>);
	AssertTrue(is_void_v<void volatile>);
	AssertTrue(is_void_v<void const volatile>);
	AssertFalse(is_void_v<int>);

	//and_
	AssertFalse(and_v<>);
	AssertTrue(and_v< is_void<void> >);
	AssertFalse(and_v< is_void<int> >);
	AssertTrue((and_v< is_void<void>, is_void<void const> >));  //both need to be true
	AssertFalse((and_v< is_void<int>, is_void<void const> >));
	AssertFalse((and_v< is_void<void>, is_void<int> >));
	AssertTrue((and_v< is_void<void>, is_void<void const>, is_void<void volatile>, is_void<void const volatile>>));

	//or_
	AssertFalse(or_v<>);
	AssertTrue(or_v< is_void<void> >);
	AssertFalse(or_v< is_void<int> >);
	AssertTrue((or_v< is_void<int>, is_void<void const> >));
	AssertTrue((or_v< is_void<void const>, is_void<int> >));
	AssertFalse((or_v< is_void<int>, is_void<int> >));        //both need to be false
	AssertTrue((or_v< is_void<void>, is_void<char>, is_void<int>, is_void<long>>));

	//add_const
	AssertTypeSame(add_const_t<int>, int const);
	AssertTypeSame(add_const_t<int const>, int const);
	AssertTypeSame(add_const_t<int volatile>, int const volatile);
	AssertTypeSame(add_const_t<int const volatile>, int const volatile);
	AssertTypeSame(add_const_t<int&>, int&);       // &  const => &
	AssertTypeSame(add_const_t<int&&>, int&&);     // && const => &&
	AssertTypeSame(add_const_t<int*>, int* const);
	AssertTypeSame(add_const_t<const int*>, const int* const);
	AssertTypeSame(add_const_t<volatile int*>, volatile int* const);
	AssertTypeSame(add_const_t<const volatile int*>, const volatile int* const);
	AssertTypeSame(add_const_t<int[2]>, int const[2]);
	AssertTypeSame(add_const_t<int[2]>, const int[2]);

	// add_top_const
	AssertTypeSame(add_top_const_t<int>, int);         //ingore
	AssertTypeSame(add_top_const_t<int*>, const int*);
	AssertTypeSame(add_top_const_t<int* volatile>, const int* volatile);
	AssertTypeSame(add_top_const_t<int* const>, const int* const);
	AssertTypeSame(add_top_const_t<int* const volatile>, const int* const volatile);
	AssertTypeSame(add_top_const_t<int&>, const int&);
	AssertTypeSame(add_top_const_t<volatile int&>, const volatile int&);
	AssertTypeSame(add_top_const_t<const int&>, const int&);
	AssertTypeSame(add_top_const_t<const volatile int&>, const volatile int&);
	AssertTypeSame(add_top_const_t<int&&>, const int&&);

	//add_volatile
	AssertTypeSame(add_volatile_t<int>, int volatile);
	AssertTypeSame(add_volatile_t<int const>, int volatile const);
	AssertTypeSame(add_volatile_t<int volatile>, int volatile);
	AssertTypeSame(add_volatile_t<int const volatile>, int const volatile);
	AssertTypeSame(add_volatile_t<int&>, int&);       // &  const => &
	AssertTypeSame(add_volatile_t<int&&>, int&&);     // && const => &&
	AssertTypeSame(add_volatile_t<int*>, int* volatile);
	AssertTypeSame(add_volatile_t<const int*>, const int* volatile);
	AssertTypeSame(add_volatile_t<volatile int*>, volatile int* volatile);
	AssertTypeSame(add_volatile_t<const volatile int*>, const volatile int* volatile);
	AssertTypeSame(add_volatile_t<int[2]>, int volatile[2]);
	AssertTypeSame(add_volatile_t<int[2]>, volatile int[2]);

	//add_cv
	AssertTypeSame(add_cv_t<int>, int const volatile);
	AssertTypeSame(add_cv_t<int const>, int volatile const);
	AssertTypeSame(add_cv_t<int volatile>, int volatile const);
	AssertTypeSame(add_cv_t<int const volatile>, int const volatile);
	AssertTypeSame(add_cv_t<int&>, int&);       // &  const => &
	AssertTypeSame(add_cv_t<int&&>, int&&);     // && const => &&
	AssertTypeSame(add_cv_t<int*>, int* volatile const);
	AssertTypeSame(add_cv_t<const int*>, const int* const volatile);
	AssertTypeSame(add_cv_t<volatile int*>, volatile int* volatile const);
	AssertTypeSame(add_cv_t<const volatile int*>, const volatile int* const volatile);
	AssertTypeSame(add_cv_t<int[2]>, int const volatile[2]);
	AssertTypeSame(add_cv_t<int[2]>, volatile const int[2]);

	//add_lref
	AssertTypeSame(add_lref_t<int>, int&);
	AssertTypeSame(add_lref_t<int&>, int&);  // & + & => &
	AssertTypeSame(add_lref_t<int&&>, int&); //&& + & => &
	AssertTypeSame(add_lref_t<int*>, int*&);
	AssertTypeSame(add_lref_t<int[2]>, int2&);
	AssertTypeSame(add_lref_t<int const>, const int&);
	AssertTypeSame(add_lref_t<void>, void);
	AssertTypeSame(add_lref_t<void const>, void const);
	AssertTypeSame(add_lref_t<void volatile>, void volatile);
	AssertTypeSame(add_lref_t<void const volatile>, void const volatile);

	//add_rref
	AssertTypeSame(add_rref_t<int>, int&&);
	AssertTypeSame(add_rref_t<int&>, int&);
	AssertTypeSame(add_rref_t<int&&>, int&&);
	AssertTypeSame(add_rref_t<void>, void);
	AssertTypeSame(add_rref_t<int[2]>, int2&&);
	AssertTypeSame(add_rref_t<int const>, const int&&);
	AssertTypeSame(add_rref_t<int volatile>, volatile int&&);

	//add pointer
	AssertTypeSame(add_pointer_t<int>, int*);
	AssertTypeSame(add_pointer_t<int*>, int**);
	AssertTypeSame(add_pointer_t<int&>, int*);
	AssertTypeSame(add_pointer_t<int&&>, int*);
	AssertTypeSame(add_pointer_t<int&>, int*);
	AssertTypeSame(add_pointer_t<int[2]>, int2*);
	AssertTypeSame(add_pointer_t<int const>, const int*);
	AssertTypeSame(add_pointer_t<int volatile>, volatile int*);
	AssertTypeSame(add_pointer_t<int const volatile>, const volatile int*);

	//add_signed
	AssertTypeSame(add_signed_t<int>, int);
	AssertTypeSame(add_signed_t<unsigned int>, int);
	AssertTypeSame(add_signed_t<unsigned int const>, int const);
	AssertTypeSame(add_signed_t<unsigned char>, signed char);
	AssertTypeSame(add_signed_t<char>, signed char);
	AssertTypeSame(add_signed_t<unsigned short>, short);
	AssertTypeSame(add_signed_t<unsigned long>, long);
	AssertTypeSame(add_signed_t<unsigned long long>, long long);

	//add_signed
	AssertTypeSame(add_unsigned_t<int>, unsigned int);
	AssertTypeSame(add_unsigned_t<char>, unsigned char);
	AssertTypeSame(add_unsigned_t<signed char>, unsigned char);
	AssertTypeSame(add_unsigned_t<int const>, unsigned int const);
	AssertTypeSame(add_unsigned_t<int volatile>, unsigned int volatile);
	AssertTypeSame(add_unsigned_t<int const volatile>, unsigned int const volatile);
	AssertTypeSame(add_unsigned_t<short>, unsigned short);
	AssertTypeSame(add_unsigned_t<long>, unsigned long);
	AssertTypeSame(add_unsigned_t<long long>, unsigned long long);

	//decay
	AssertTypeSame(int, decay_t<int>);
	AssertTypeSame(int*, decay_t<int[2]>);
	AssertTypeSame(int(*)(int), decay_t<int(int)>);
	AssertTypeSame(int, decay_t<const int>);
	AssertTypeSame(int, decay_t<int&>);
	AssertTypeSame(int, decay_t<const int&>);

	//array dim
	AssertTrue(0 == array_dim_v< int >);
	AssertTrue(0 == array_dim_v< int* >);
	AssertTrue(1 == array_dim_v< int[2] >);
	AssertTrue(2 == array_dim_v< int[2][1] >);
	AssertTrue(3 == array_dim_v< int[2][3][4] >);
	AssertTrue(1 == array_dim_v< int[] >);
	AssertTrue(2 == array_dim_v< int[][1] >);
	AssertTrue(3 == array_dim_v< int[][1][1] >);

	//array_len
	AssertTrue(0 == array_len_v< int >);
	AssertTrue(0 == array_len_v< int* >);
	AssertTrue(2 == array_len_v< int[2] >);
	AssertTrue(2 == array_len_v< int[2][1] >);
	AssertTrue(2 == array_len_v< int[2][3][4] >);
	AssertTrue(6 == array_len_v< int[6] >);
	AssertTrue(0 == array_len_v< int[] >);

	//remove_all_dim
	AssertTypeSame(remove_all_dim_t< int>, int);
	AssertTypeSame(remove_all_dim_t< int*>, int*);
	AssertTypeSame(remove_all_dim_t< int[]>, int);
	AssertTypeSame(remove_all_dim_t< int[1]>, int);
	AssertTypeSame(remove_all_dim_t< int[1][2]>, int);
	AssertTypeSame(remove_all_dim_t< int[][1]>, int);

	//remove_dim
	AssertTypeSame(remove_dim_t< int>, int);
	AssertTypeSame(remove_dim_t< int*>, int*);
	AssertTypeSame(remove_dim_t< const int[]>, const int);
	AssertTypeSame(remove_dim_t< int const[2]>, const int);
	AssertTypeSame(remove_dim_t< int[]>, int);
	AssertTypeSame(remove_dim_t< int[1]>, int);
	AssertTypeSame(remove_dim_t< int[1][2]>, int[2]);
	AssertTypeSame(remove_dim_t< int[][1]>, int[1]);

	//remove_cv
	AssertTypeSame(remove_cv_t<int>, int);
	AssertTypeSame(remove_cv_t<int const>, int);
	AssertTypeSame(remove_cv_t<int volatile>, int);
	AssertTypeSame(remove_cv_t<int const volatile>, int);
	AssertTypeSame(remove_cv_t<const int&>, const int&);
	AssertTypeSame(remove_cv_t<const int*>, const int*);

	//remove_top_const
	AssertTypeSame(remove_top_const_t<int>, int);
	AssertTypeSame(remove_top_const_t<int const>, int const);
	AssertTypeSame(remove_top_const_t<const int>, const int); //same as above
	AssertTypeSame(remove_top_const_t<const int*>, int*);
	AssertTypeSame(remove_top_const_t<const int* const >, int* const);
	AssertTypeSame(remove_top_const_t<const int* volatile >, int* volatile);
	AssertTypeSame(remove_top_const_t<const int* const volatile >, int* const volatile);
	AssertTypeSame(remove_top_const_t<const int&>, int&);

	//remove const
	AssertTypeSame(remove_const_t<int>, int);
	AssertTypeSame(remove_const_t<const int>, int);
	AssertTypeSame(remove_const_t<const volatile int>, volatile int);
	AssertTypeSame(remove_const_t<volatile int>, volatile int);
	AssertTypeSame(remove_const_t<int const[2]>, int[2]);
	AssertTypeSame(remove_const_t<int* const>, int*);
	AssertTypeSame(remove_const_t<const int*>, const int*);

	//remove volatile
	AssertTypeSame(remove_volatile_t<int>, int);
	AssertTypeSame(remove_volatile_t<volatile int>, int);
	AssertTypeSame(remove_volatile_t<const volatile int>, const int);
	AssertTypeSame(remove_volatile_t<const int>, const int);
	AssertTypeSame(remove_volatile_t<int volatile[2]>, int[2]);
	AssertTypeSame(remove_volatile_t<int* volatile>, int*);
	AssertTypeSame(remove_volatile_t<volatile int*>, volatile int*);

	//remove_ref
	AssertTypeSame(remove_ref_t<int>, int);
	AssertTypeSame(remove_ref_t<int&>, int);
	AssertTypeSame(remove_ref_t<int&&>, int);
	AssertTypeSame(remove_ref_t<const int&>, int const);
	AssertTypeSame(remove_ref_t<volatile int&>, int volatile);
	AssertTypeSame(remove_ref_t<const volatile int&>, int volatile const);
	AssertTypeSame(remove_ref_t<const int*>, int const*);

	//remove pointer
	AssertTypeSame(remove_pointer_t<int>, int);
	AssertTypeSame(remove_pointer_t<int*>, int);
	AssertTypeSame(remove_pointer_t<int**>, int*);
	AssertTypeSame(remove_pointer_t<const int*>, const int);
	AssertTypeSame(remove_pointer_t<int* const>, int);
	AssertTypeSame(remove_pointer_t<int*>, int);


	return 0;
}