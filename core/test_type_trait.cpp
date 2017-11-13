#include<cstdio>
#include<utility>
#include<memory>
#include<string>
#include<typeinfo>

#include<core/preprocessor/stringize.h>

#include<core/mpl/base/and_.h>
#include<core/mpl/base/or_.h>
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
#include<core/mpl/type_traits/is_compatible.h>

#include<core/mpl/type_traits/add_const.h>
#include<core/mpl/type_traits/add_volatile.h>
#include<core/mpl/type_traits/add_cv.h>
#include<core/mpl/type_traits/add_lref.h>
#include<core/mpl/type_traits/add_rref.h>
#include<core/mpl/type_traits/add_top_const.h>
#include<core/mpl/type_traits/add_signed.h>
#include<core/mpl/type_traits/add_unsigned.h>
#include<core/mpl/type_traits/add_pointer.h>
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

#include<core/mpl/type_traits/has_add.h>
#include<core/mpl/type_traits/has_sub.h>
#include<core/mpl/type_traits/has_mul.h>
#include<core/mpl/type_traits/has_div.h>
#include<core/mpl/type_traits/has_mod.h>
#include<core/mpl/type_traits/has_bit_or.h>
#include<core/mpl/type_traits/has_bit_and.h>
#include<core/mpl/type_traits/has_bit_xor.h>
#include<core/mpl/type_traits/has_bit_reverse.h>
#include<core/mpl/type_traits/has_shl.h>
#include<core/mpl/type_traits/has_shr.h>
#include<core/mpl/type_traits/has_lequal.h>
#include<core/mpl/type_traits/has_nequal.h>
#include<core/mpl/type_traits/has_gequal.h>
#include<core/mpl/type_traits/has_equal.h>
#include<core/mpl/type_traits/has_less.h>
#include<core/mpl/type_traits/has_greater.h>
#include<core/mpl/type_traits/has_logic_and.h>
#include<core/mpl/type_traits/has_logic_or.h>

#include<core/mpl/type_traits/has_front_dec.h>
#include<core/mpl/type_traits/has_front_inc.h>
#include<core/mpl/type_traits/has_post_dec.h>
#include<core/mpl/type_traits/has_post_inc.h>
#include<core/mpl/type_traits/has_positive.h>
#include<core/mpl/type_traits/has_negative.h>
#include<core/mpl/type_traits/has_deref.h>

#include<core/mpl/type_traits/has_delete.h>
#include<core/mpl/type_traits/has_new.h>
#include<core/mpl/type_traits/has_indexer.h>
#include<core/mpl/type_traits/has_invoker.h>
#include<core/mpl/type_traits/has_arrow.h>
#include<core/mpl/type_traits/has_inner_type.h>
#include<core/mpl/type_traits/compatible_type.h>

#include<core/mpl/type_traits/has_constructor.h>
#include<core/mpl/type_traits/has_destructor.h>
#include<core/mpl/type_traits/has_assigner.h>

#include<core/mpl/base/dispatch_when.h>


//using namespace std;
using namespace core::mpl;

#define AssertFalse(v) static_assert(!v, "")
#define AssertTrue(v) static_assert( v, "")


typedef int int2[2];

void varg_fn(...){}
void __cdecl varg_cd_fn(...) {}
void __stdcall varg_sc_fn(...) {}
//void __vectorcall varg_vc_fn(...) {}
void __fastcall varg_fc_fn(...) {}
void normal_fn() {}
void __cdecl normal_cd_fn() {}
void __stdcall normal_sc_fn() {}
void __vectorcall normal_vc_fn() {}
void __fastcall normal_fc_fn() {}

class TestMemberClass {
private: int private_obj;
public: int public_obj;
		const int const_public_obj =1;
		volatile int volatile_public_obj;
		const volatile int cv_public_obj =2;
		static int static_obj;
		static int static_fn() {};
		void member_fn() {};
		void member_noexcept_fn() noexcept {};
		//void member_noexcept_fn() {}
		void member_const_fn() const { printf("const member fn\n"); };
		void member_const_fn() volatile { printf("volatile member fn\n"); };
		void member_const_fn() const volatile { printf("const volatile member fn\n"); };
		void member_const_fn() { printf(" non-const member fn\n");  };
		constexpr void member_constexpr_fn(){};
		void member_ref_fn() const& { printf("const& member fn\n"); };
		void member_ref_fn() & { printf("& member fn\n");  };
		void member_ref_fn() && { printf("&& member fn\n");  };
		void member_varg_fn(...) {};
};

union TestUnion{};
class TestAbstract { virtual void test() {} };
class TestDerive :public TestAbstract { virtual void test() override final{} };


void output_test_info();
void operator delete(void* mem)
{
	printf("replace delete\n");
	//::delete(mem);
}

void operator delete[](void * mem)
{
	printf("replace delete[]\n");
	//::delete[](mem);
}

namespace detail2
{
	class TestHasDelete2
	{
	public:
		void* operator new(size_t size) {}
		void* operator new(size_t size, void* ptr) {}
		void* operator new(size_t size, int) {}
		void* operator new[](size_t size) {}
		void operator delete(void * ptr) {}
		void operator delete(void* ptr, size_t size) {}
		void operator delete[](void* ptr) {}
		void operator delete[](void* ptr, size_t size) {}
	};
};

#include<type_traits>
int main()
{
	//test has constructor
	struct TestConstructor { TestConstructor() {}; };
	struct TestNothrowConstructpr { TestNothrowConstructpr() noexcept {} };
	struct TestDefaultConstructor1 {};
	struct TestDefaultConstructor2 { TestDefaultConstructor2() = default; };
	struct TestArgConstructor { TestArgConstructor(int) {} };
	struct TestNoCopyConstructor { TestNoCopyConstructor(const TestNoCopyConstructor&) = delete; };
	struct TestCopyConstructor { TestCopyConstructor(const TestCopyConstructor&) {} };
	struct TestRefConstructor { TestRefConstructor(TestRefConstructor&) {} };
	struct TestMoveConstructor { TestMoveConstructor(TestMoveConstructor&&) {} };
	AssertTrue((has_constructor_v<TestConstructor>));
	AssertTrue((has_constructor_v<TestDefaultConstructor1>));
	AssertTrue((has_constructor_v<TestDefaultConstructor2>));
	AssertTrue((has_constructor_v<TestArgConstructor,int>));
	AssertFalse((has_constructor_v<TestArgConstructor, int,int>));
	AssertFalse((has_constructor_v<TestArgConstructor>));
	AssertTrue((has_copy_constructor_v<TestArgConstructor>));
	AssertFalse((has_copy_constructor_v<TestNoCopyConstructor>));
	AssertFalse((has_copy_constructor_v<TestRefConstructor>));
	AssertFalse((has_copy_constructor_v<TestMoveConstructor>));
	AssertTrue((has_move_constructor_v<TestArgConstructor>));
	AssertFalse((has_move_constructor_v<TestNoCopyConstructor>));
	AssertTrue((has_move_constructor_v<TestRefConstructor>));
	AssertTrue((has_move_constructor_v<TestMoveConstructor>));
	AssertFalse((has_nothrow_constructor_v<TestArgConstructor>));
	AssertTrue((has_nothrow_constructor_v<TestNothrowConstructpr>));

	//bool wrap
	AssertTrue(true_::value);
	AssertTrue(true_c);
	AssertFalse(false_::value);
	AssertFalse(false_c);

	//is_same
	AssertTrue((is_same_v<int, int>) );
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
	AssertTypeSame(add_const_t<int[2]>, const int [2]);

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
	AssertTypeSame(add_lref_t<int const>, const int& );
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

	//is float
	AssertTrue(is_float_v<float>);
	AssertFalse(is_float_v<float&>);
	AssertFalse(is_float_v<float*>);
	AssertTrue(is_float_v<float const>);
	AssertTrue(is_float_v<float volatile>);
	AssertTrue(is_float_v<float const volatile>);
	AssertTrue(is_float_v<double>);
	AssertTrue(is_float_v<long double>);
	AssertFalse(is_float_v<int>);

	//is integer
	AssertTrue(is_integer_v<int>);
	AssertTrue(is_integer_v<bool>);
	AssertTrue(is_integer_v<int const>);
	AssertTrue(is_integer_v<int volatile>);
	AssertTrue(is_integer_v<int const volatile>);
	AssertTrue(is_integer_v<char>);
	AssertTrue(is_integer_v<signed>);
	AssertTrue(is_integer_v<unsigned char>);
	AssertTrue(is_integer_v<short>);
	AssertTrue(is_integer_v<unsigned short>);
	AssertTrue(is_integer_v<unsigned int>);
	AssertTrue(is_integer_v<unsigned long>);
	AssertTrue(is_integer_v<long>);
	AssertTrue(is_integer_v<long long>);
	AssertTrue(is_integer_v<unsigned long long>);

	//is unsigned 
	AssertTrue(is_unsigned_v<unsigned char>);
	AssertTrue(is_unsigned_v<unsigned char volatile>);
	AssertTrue(is_unsigned_v<unsigned char volatile const>);
	AssertTrue(is_unsigned_v<unsigned char const>);
	AssertTrue(is_unsigned_v<unsigned short>);
	AssertTrue(is_unsigned_v<unsigned int>);
	AssertTrue(is_unsigned_v<unsigned long>);
	AssertTrue(is_unsigned_v<unsigned long long>);
	AssertFalse(is_unsigned_v<signed char>);
	AssertFalse(is_unsigned_v<char>);
	AssertFalse(is_unsigned_v<signed short>);
	AssertFalse(is_unsigned_v<signed int>);
	AssertFalse(is_unsigned_v<signed long>);
	AssertFalse(is_unsigned_v<signed long long>);
	AssertFalse(is_unsigned_v<int*>);
	AssertFalse(is_unsigned_v<int&>);
	AssertFalse(is_unsigned_v<int[]>);
	AssertFalse(is_unsigned_v< int_<2> >);

	//is signed
	AssertTrue(is_signed_v<signed char>);
	AssertTrue(is_signed_v<signed char volatile>);
	AssertTrue(is_signed_v<signed char const>);
	AssertTrue(is_signed_v<signed char const volatile>);
	AssertTrue(is_signed_v<signed short>);
	AssertTrue(is_signed_v<signed int>);
	AssertTrue(is_signed_v<signed long>);
	AssertTrue(is_signed_v<signed long long>);
	AssertTrue(is_signed_v<char>);
	AssertFalse(is_signed_v<unsigned short>);
	AssertFalse(is_signed_v<unsigned int>);
	AssertFalse(is_signed_v<unsigned long>);
	AssertFalse(is_signed_v<unsigned long long>);
	AssertFalse(is_signed_v<int*>);
	AssertFalse(is_signed_v<int&>);
	AssertFalse(is_signed_v<int[]>);
	AssertFalse(is_signed_v< int_<2> >);

	//is nullptr_t
	AssertTrue(is_nullptr_v<nullptr_t>);
	AssertTrue(is_nullptr_v<nullptr_t const>);
	AssertTrue(is_nullptr_v<nullptr_t volatile>);
	AssertTrue(is_nullptr_v<nullptr_t const volatile>);
	AssertTrue(is_nullptr_v<decltype(nullptr)>);
	AssertFalse(is_nullptr_v<int>);

	//is instance of
	AssertTrue(is_instance_of(2, int) ); 
	AssertTrue(is_instance_of('a', char) );
	AssertFalse(is_instance_of(2.5f, int));
	int a = 1;
	AssertTrue(is_instance_of(a, int));
	AssertTrue(is_instance_of(nullptr, nullptr_t));

	//is fundamental
	AssertTrue(is_fundamental_v<bool>);
	AssertTrue(is_fundamental_v<nullptr_t>);
	AssertTrue(is_fundamental_v<void>);
	AssertTrue(is_fundamental_v<char>);
	AssertTrue(is_fundamental_v<signed char>);
	AssertTrue(is_fundamental_v<unsigned char>);
	AssertTrue(is_fundamental_v<short>);
	AssertTrue(is_fundamental_v<unsigned short>);
	AssertTrue(is_fundamental_v<int>);
	AssertTrue(is_fundamental_v<long>);
	AssertTrue(is_fundamental_v<unsigned long>);
	AssertTrue(is_fundamental_v<long long>);
	AssertTrue(is_fundamental_v<unsigned long long>);
	AssertTrue(is_fundamental_v<float>);
	AssertTrue(is_fundamental_v<double>);
	AssertTrue(is_fundamental_v<long double>);
	AssertTrue(is_fundamental_v<float volatile>);
	AssertTrue(is_fundamental_v<float const>);
	AssertTrue(is_fundamental_v<float const volatile>);
	AssertFalse(is_fundamental_v<int_<1>>);

	
	//enum_base
	enum TestNewEnum :unsigned char {};
	enum TestOldEnum {};
	AssertTypeSame(enum_base_t<TestNewEnum>, unsigned char);
	AssertTypeSame(enum_base_t<TestOldEnum>, int);
	AssertTypeSame(enum_base_t<char>, char);
	AssertTrue(sizeof(TestNewEnum) == 1);
	AssertTrue(sizeof(TestOldEnum) == 4);

	//is member pointer / is mobj pointer / is memfn pointer
	const TestMemberClass e{};
	void (TestMemberClass::*p_member_const_fn1)() const = &TestMemberClass::member_const_fn;
	void (TestMemberClass::*p_member_const_fn2)() volatile = &TestMemberClass::member_const_fn;
	void (TestMemberClass::*p_member_const_fn3)() const volatile = &TestMemberClass::member_const_fn;
	void (TestMemberClass::*p_member_const_fn4)() = &TestMemberClass::member_const_fn;
	void (TestMemberClass::*p_member_ref_fn1)() && = &TestMemberClass::member_ref_fn;
	void (TestMemberClass::*p_member_ref_fn2)() & = &TestMemberClass::member_ref_fn;
	void (TestMemberClass::*p_member_ref_fn3)() const& = &TestMemberClass::member_ref_fn;
	constexpr void (TestMemberClass::*p_member_noexcept_fn3)() noexcept = &TestMemberClass::member_noexcept_fn;
	//is member pointer
	AssertTrue(is_member_ptr_v<decltype(&TestMemberClass::member_fn)>);
	AssertTrue(is_member_ptr_v<decltype(&TestMemberClass::member_fn) const>);
	AssertTrue(is_member_ptr_v<decltype(&TestMemberClass::member_fn) volatile>);
	AssertTrue(is_member_ptr_v<decltype(&TestMemberClass::member_fn) const volatile>);
	AssertTrue(is_member_ptr_v<decltype(&TestMemberClass::member_noexcept_fn)>);
	AssertTrue(is_member_ptr_v<decltype(&TestMemberClass::public_obj)>);
	AssertTrue(is_member_ptr_v<decltype(p_member_const_fn1)>);
	AssertTrue(is_member_ptr_v<decltype(p_member_const_fn2)>);
	AssertTrue(is_member_ptr_v<decltype(p_member_const_fn3)>);
	AssertTrue(is_member_ptr_v<decltype(p_member_const_fn4)>);
	AssertTrue(is_member_ptr_v<decltype(p_member_ref_fn1)>);
	AssertTrue(is_member_ptr_v<decltype(p_member_ref_fn2)>);
	AssertTrue(is_member_ptr_v<decltype(p_member_ref_fn3)>);
	AssertFalse(is_member_ptr_v<decltype(&TestMemberClass::static_fn)>);
	AssertFalse(is_member_ptr_v<decltype(&TestMemberClass::static_obj)>);
	AssertFalse(is_member_ptr_v<decltype(normal_fn)>);
	AssertFalse(is_member_ptr_v<decltype(varg_fn)>);

	//is member object pointer
	AssertTrue(is_mobj_ptr_v<decltype(&TestMemberClass::public_obj)>);
	AssertTrue(is_mobj_ptr_v<decltype(&TestMemberClass::volatile_public_obj)>);
	AssertTrue(is_mobj_ptr_v<decltype(&TestMemberClass::cv_public_obj)>);
	AssertTrue(is_mobj_ptr_v<decltype(&TestMemberClass::const_public_obj)>);
	AssertFalse(is_mobj_ptr_v<decltype(&TestMemberClass::member_fn)>);
	AssertFalse(is_mobj_ptr_v<int>);
	AssertFalse(is_mobj_ptr_v<decltype(&TestMemberClass::static_obj)>);
	AssertFalse(is_mobj_ptr_v<decltype(normal_fn)>);
	AssertFalse(is_mobj_ptr_v<decltype(varg_fn)>);

	
	//is member function pointer
	AssertTrue(is_mfn_ptr_v<decltype(&TestMemberClass::member_constexpr_fn)>);
	AssertTrue(is_mfn_ptr_v<decltype(&TestMemberClass::member_fn)>);
	AssertTrue(is_mfn_ptr_v<decltype(&TestMemberClass::member_noexcept_fn)>);
	AssertTrue(is_mfn_ptr_v<decltype(&TestMemberClass::member_varg_fn)>);
	AssertTrue(is_mfn_ptr_v<decltype(p_member_const_fn1)>);
	AssertTrue(is_mfn_ptr_v<decltype(p_member_const_fn2)>);
	AssertTrue(is_mfn_ptr_v<decltype(p_member_const_fn3)>);
	AssertTrue(is_mfn_ptr_v<decltype(p_member_const_fn4)>);
	AssertTrue(is_mfn_ptr_v<decltype(p_member_ref_fn1)>);
	AssertTrue(is_mfn_ptr_v<decltype(p_member_ref_fn2)>);
	AssertTrue(is_mfn_ptr_v<decltype(p_member_ref_fn3)>);
	AssertFalse(is_mfn_ptr_v<decltype(&TestMemberClass::public_obj)>);
	AssertFalse(is_mfn_ptr_v<decltype(&TestMemberClass::static_fn)>);
	AssertFalse(is_mfn_ptr_v<decltype(&TestMemberClass::static_obj)>);
	AssertFalse(is_mfn_ptr_v<decltype(normal_fn)>);
	AssertFalse(is_mfn_ptr_v<decltype(varg_fn)>);

	//is function
	void(*normal_p)() = normal_cd_fn;
	void( *const normal_const)() = normal_cd_fn;
	void(*volatile normal_volatile)() = normal_cd_fn;
	void(*const volatile normal_cv)() = normal_cd_fn;
	AssertTrue(is_function_v<decltype(normal_fn)>);
	//AssertTrue(is_function_v<decltype(*normal_p)>);
	//AssertTrue(is_function_v<decltype(normal_const)>);
	//AssertTrue(is_function_v<decltype(normal_volatile)>);
	//AssertTrue(is_function_v<decltype(normal_cv)>);
	AssertTrue(is_function_v<decltype(normal_cd_fn)>);
	AssertTrue(is_function_v<decltype(normal_sc_fn)>);
	AssertTrue(is_function_v<decltype(normal_fc_fn)>);
	AssertTrue(is_function_v<decltype(normal_vc_fn)>);
	AssertTrue(is_function_v<decltype(varg_fn)>);
	AssertTrue(is_function_v<decltype(varg_cd_fn)>);
	AssertTrue(is_function_v<decltype(varg_sc_fn)>);
	AssertTrue(is_function_v<decltype(varg_fc_fn)>);
	
	//is scaler
	AssertTrue(is_scaler_v<int*>);
	AssertTrue(is_scaler_v<int* const>);
	AssertTrue(is_scaler_v<int* volatile>);
	AssertTrue(is_scaler_v<int* const volatile>);
	AssertTrue(is_scaler_v<int>);
	AssertTrue(is_scaler_v<float>);
	AssertTrue(is_scaler_v<TestNewEnum>);
	AssertTrue(is_scaler_v<decltype(&normal_fn)>);
	AssertTrue(is_scaler_v<decltype(&TestMemberClass::public_obj)>);
	AssertTrue(is_scaler_v<decltype(&TestMemberClass::member_noexcept_fn)>);
	AssertTrue(is_scaler_v<decltype(p_member_const_fn1)>);
	AssertTrue(is_scaler_v<decltype(p_member_const_fn2)>);
	AssertTrue(is_scaler_v<decltype(p_member_const_fn3)>);
	AssertTrue(is_scaler_v<decltype(p_member_const_fn4)>);
	AssertTrue(is_scaler_v<decltype(p_member_ref_fn1)>);
	AssertTrue(is_scaler_v<decltype(p_member_ref_fn2)>);
	AssertTrue(is_scaler_v<decltype(p_member_ref_fn3)>);
	
	//is union
	AssertTrue(is_union_v<TestUnion>);
	AssertTrue(is_union_v<TestUnion const>);
	AssertTrue(is_union_v<TestUnion volatile>);
	AssertTrue(is_union_v<TestUnion const volatile>);
	AssertFalse(is_union_v<TestDerive>);
	AssertFalse(is_union_v<TestAbstract>);
	AssertFalse(is_union_v<TestNewEnum>);

	//is l ref
	AssertTrue(is_lref_v<TestUnion&>);
	AssertTrue(is_lref_v<int&>);
	AssertTrue(is_lref_v<const int&>);
	AssertTrue(is_lref_v<volatile int&>);
	AssertTrue(is_lref_v<const volatile int&>);
	AssertFalse(is_lref_v<float>);
	AssertFalse(is_lref_v<TestUnion&&>);

	//is r ref
	AssertTrue(is_rref_v<TestUnion&&>);
	AssertTrue(is_rref_v<int&&>);
	AssertTrue(is_rref_v<const int&&>);
	AssertTrue(is_rref_v<volatile int&&>);
	AssertTrue(is_rref_v<const volatile int&&>);
	AssertFalse(is_rref_v<TestUnion&>);
	AssertFalse(is_rref_v<int&>);

	//is ref
	AssertTrue(is_ref_v<TestUnion&&>);
	AssertTrue(is_ref_v<int&&>);
	AssertTrue(is_ref_v<TestUnion&>);
	AssertTrue(is_ref_v<int&>);
	AssertTrue(is_ref_v<const int&>);
	AssertTrue(is_ref_v<volatile int&>);
	AssertTrue(is_ref_v<const volatile int&>);
	AssertTrue(is_ref_v<const int&&>);
	AssertTrue(is_ref_v<volatile int&&>);
	AssertTrue(is_ref_v<const volatile int&&>);

	//is pointer
	AssertTrue(is_pointer_v<int*>);
	AssertTrue(is_pointer_v<int* const>);
	AssertTrue(is_pointer_v<int* volatile>);
	AssertTrue(is_pointer_v<int* const volatile>);
	AssertTrue(is_pointer_v<const int*>);
	AssertTrue(is_pointer_v<volatile int*>);
	AssertTrue(is_pointer_v<const volatile int*>);
	AssertTrue(is_pointer_v<decltype(&varg_fn)>);
	AssertTrue(is_pointer_v<decltype(&normal_fn)>);
	AssertTrue(is_pointer_v<int*>);
	AssertFalse(is_pointer_v<int&>);
	AssertFalse(is_pointer_v<int&&>);

	//is array
	AssertTrue(is_array_v<int[]>);
	AssertTrue(is_array_v<int[1]>);
	AssertTrue(is_array_v<int[][1]>);
	AssertTrue(is_array_v<int[1][1]>);

	
	//is trivial
	class T1 { int a = 1; };
	AssertTrue(is_trivial_v<int>);
	AssertTrue(is_trivial_v<int*>);
	AssertTrue(is_trivial_v<int*>);
	AssertFalse(is_trivial_v<T1>);
	AssertFalse((is_trivial_v<std::pair<int,int>>));
	AssertFalse((is_trivial_v<std::unique_ptr<int> >));

	//is standart layout
	class B1 {};
	class B2:public B1 { int a; };
	class B3:public B2 {};
	class B4:public B3 { int b; };
	AssertTrue(is_standlayout_v<int>);
	AssertTrue(is_standlayout_v<B1>);
	AssertTrue(is_standlayout_v<B2>);
	AssertTrue(is_standlayout_v<B3>);
	AssertFalse(is_standlayout_v<B4>);
	AssertTrue((is_standlayout_v<std::pair<int, int>>));
	AssertTrue((is_standlayout_v<std::unique_ptr<int>>));

	//is polymorphic
	class C1 { virtual void test() {} };
	class C2:public C1 { virtual void test() {} };
	AssertFalse(is_polymorphic_v<int>);
	AssertFalse(is_polymorphic_v<B4>);
	AssertFalse(is_polymorphic_v<C2&>);
	AssertTrue(is_polymorphic_v<C2>);
	AssertTrue(is_polymorphic_v<C2 const>);
	AssertTrue(is_polymorphic_v<C2 volatile>);
	AssertTrue(is_polymorphic_v<C2 const volatile>);
	AssertTrue(is_polymorphic_v<C1>);

	//is abstract
	class C3 { virtual void test() = 0; };
	AssertFalse(is_abstract_v<C1>);
	AssertFalse(is_abstract_v<C2>);
	AssertTrue(is_abstract_v<C3>);
	AssertTrue(is_abstract_v<C3 const>);
	AssertTrue(is_abstract_v<C3 volatile>);
	AssertTrue(is_abstract_v<C3 const volatile>);

	//is base/ is child
	AssertTrue((is_base_of_v<C1, C2>));
	AssertTrue((is_base_of_v<C1, C2 const>));
	AssertTrue((is_base_of_v<C1, C2 volatile>));
	AssertTrue((is_base_of_v<C1, C2 const volatile>));
	AssertTrue((is_base_of_v<C1, C1>));
	AssertTrue((is_child_of_v<C2, C1>));
	AssertTrue((is_child_of_v<C2, C1 const>));
	AssertTrue((is_child_of_v<C2, C1 volatile>));
	AssertTrue((is_child_of_v<C2, C1 const volatile>));
	AssertFalse((is_child_of_v<C1, C1>));

	//is final
	class D1 final {};
	AssertTrue(is_final_v<D1>);
	AssertTrue(is_final_v<D1 const>);
	AssertTrue(is_final_v<D1 volatile>);
	AssertTrue(is_final_v<D1 const volatile>);
	AssertFalse(is_final_v<int>);

	//is empty
	AssertTrue(is_empty_v<D1 >);
	AssertTrue(is_empty_v<D1 const>);
	AssertTrue(is_empty_v<D1 volatile>);
	AssertTrue(is_empty_v<D1 const volatile>);
	AssertFalse(is_empty_v<TestUnion>);  //??
	AssertFalse(is_empty_v<int>);

	//is convertible
	AssertTrue((is_convertible_v<int, float>));
	AssertTrue((is_convertible_v<float, int>));
	AssertFalse((is_convertible_v<C1*, C2*>));
	AssertTrue((is_convertible_v<C1*, void*>));
	AssertFalse((is_convertible_v<C1*, int>));
	AssertTrue((is_convertible_v<C2*, C1*>));    //derived* -> base*
	AssertTrue((is_convertible_v<C2&, C1&>));    //derived& -> base&
	AssertFalse((is_convertible_v<C1&, C2&>));   
	AssertTrue((is_convertible_v<const char*, std::string>)); //explicit constructor
	AssertTrue((is_convertible_v<int_<0>, int>)); //explicit convert function

	//is const
	AssertFalse(is_const_v<int>);
	AssertTrue(is_const_v<const int>);
	AssertFalse(is_const_v<const int&>); //top const
	AssertFalse(is_const_v<const int*>); //top const
	AssertTrue(is_const_v<int* const>);  //

	//is class
	AssertTrue(is_class_v<TestAbstract>);
	AssertTrue(is_class_v<TestAbstract const>);
	AssertTrue(is_class_v<TestAbstract volatile>);
	AssertTrue(is_class_v<TestAbstract const volatile>);
	AssertTrue(is_class_v<TestDerive>);
	AssertFalse(is_class_v<TestUnion>);  //distinct with  is_union
	AssertFalse(is_class_v<int>);

	//is compatible
	class A {};
	AssertTrue((is_compatible_v<int, float>));
	AssertFalse((is_compatible_v<int, void>));
	AssertFalse((is_compatible_v<int, A>));
	AssertFalse((is_compatible_v<int, float, A>));

	// has_add
	class TestAdd { public: void operator+(int a) {} };
	AssertFalse((has_add_v<int, int, int const>));
	AssertFalse((has_add_v<int, int, int&&>));
	AssertFalse((has_add_v<int, int, TestAdd>));
	AssertTrue((has_add_v<TestAdd, int>));
	AssertTrue((has_add_v<TestAdd, float>));      //implicit convert to int
	AssertFalse((has_add_v<float, TestAdd>));     
	AssertTrue((has_add_v<int, int>));
	AssertFalse((has_add_v<char*, int>));          //T* + integer is ill-formed
	AssertFalse((has_add_v<void*, int>));         //void* + integer is ill-formed
	AssertFalse((has_add_v<TestAdd*, TestAdd*>)); //T* + P* is ill-formed
	AssertFalse((has_add_v<int*&, int*&>));

	// has_add_assign
	AssertTrue((has_add_assign_v<int, int>));
	AssertFalse((has_add_assign_v<const int, int>)); //const lvalue + T is ill-formed
	AssertFalse((has_add_assign_v<int, int*>));

	//has_mul
	class TestMul { public: void operator*(int a) {} };
	AssertTrue((has_mul_v<TestMul, int>));
	AssertFalse((has_mul_v<int, TestMul>));
	AssertFalse((has_mul_v<int*, int*>));    
	AssertFalse((has_mul_v<int*, int>));

	//has mul assign
	AssertFalse((has_mul_assign_v<TestMul, int>));
	AssertTrue((has_mul_assign_v<int, int>));
	AssertFalse((has_mul_assign_v<const int, int>));

	//has bit and
	class TestBitAnd { public: void operator&(int a) {} };
	AssertTrue((has_bit_and_v<TestBitAnd, int>));
	AssertFalse((has_bit_and_v<int, TestBitAnd>));
	AssertTrue((has_bit_and_v<int, int>));
	AssertFalse((has_bit_and_v<float, int>));  // 
	AssertFalse((has_bit_and_v<void, int>));   // non-fundamental & integer
	AssertFalse((has_bit_and_v<float*, int>)); // pointer & fundamental

	// has bit reverse
	class TestBitReverse { public: TestBitReverse operator~() {} };
	AssertTrue(has_bit_reverse_v<int>);
	AssertFalse(has_bit_reverse_v<int*>);
	AssertFalse(has_bit_reverse_v<void>);
	AssertFalse(has_bit_reverse_v<nullptr_t>);
	AssertTrue(has_bit_reverse_v<TestBitReverse>);

	//has cmp 
	AssertTrue((has_equal_v<int, int>));
	AssertFalse((has_equal_v<int, TestDerive>));
	AssertFalse((has_equal_v<int, TestDerive*>));
	AssertTrue((has_equal_v<TestDerive*, TestAbstract*>));

	//has logic and
	class TestLogic { public: constexpr operator bool() { return false; } };
	AssertTrue((has_logic_and_v<int, int>));
	AssertTrue((has_logic_and_v<int, int*>));
	AssertFalse((has_logic_and_v<int, void>));
	AssertFalse((has_logic_and_v<int, TestBitAnd>));
	AssertFalse((has_logic_and_v<int, TestBitAnd>));
	AssertTrue((has_logic_and_v<int, TestLogic>));

	//has front inc
	class TestFrontInc { public: TestFrontInc operator++(){} };
	class TestBackInc { public: TestFrontInc operator++(int) {} };
	AssertTrue(has_front_inc_v<int>);
	AssertTrue(has_front_inc_v<float>);
	AssertTrue(has_front_inc_v<int*>);
	AssertTrue(has_front_inc_v<TestFrontInc>);
	AssertFalse(has_front_inc_v<TestBackInc>);
	AssertFalse(has_front_inc_v<TestNewEnum>); //enum no op
	AssertFalse(has_front_inc_v<int const>);   //const can't change self
	AssertFalse(has_front_inc_v<int&&>);       //not changable lvalue
	AssertFalse(has_front_inc_v<void*>);       
	AssertFalse(has_front_inc_v<void>);       
	AssertFalse(has_front_inc_v<nullptr_t>);       

	//has post inc
	AssertTrue(has_post_inc_v<int>);
	AssertTrue(has_post_inc_v<float>);
	AssertTrue(has_post_inc_v<int*>); 
	AssertFalse(has_post_inc_v<TestFrontInc>);
	AssertTrue(has_post_inc_v<TestBackInc>);
	AssertFalse(has_post_inc_v<TestNewEnum>); //enum no op
	AssertFalse(has_post_inc_v<int const>);   //const can't change self
	AssertFalse(has_post_inc_v<int&&>);       //not changable lvalue
	AssertFalse(has_post_inc_v<void*>);
	AssertFalse(has_post_inc_v<void>);
	AssertFalse(has_post_inc_v<nullptr_t>);
	
	//has positive/negative
	class TestPositive { public: TestPositive operator+() {} };
	class TestPositiveRet { public: TestPositiveRet(int) {} };
	AssertTrue(has_positive_v<int>);
	AssertTrue((has_positive_v<int, int>));
	AssertFalse((has_positive_v<int, const int>));   //const  ret
	AssertFalse((has_positive_v<int, int&&>));       //rvalue ret
	AssertFalse((has_positive_v<int,TestPositive>)); //no constructor
	AssertTrue((has_positive_v<int, TestPositiveRet>)); //has constructor
	AssertTrue(has_positive_v<const int>);
	AssertTrue(has_positive_v<TestPositive>);
	AssertTrue(has_positive_v<int*>);
	AssertTrue(has_positive_v<TestNewEnum>);

	//test has deref
	class TestDeref { public: int operator*() {} };
	AssertTrue(has_deref_v<TestDeref>);
	AssertFalse(has_deref_v<int>);
	AssertTrue(has_deref_v<decltype(varg_fn)>);
	AssertFalse(has_deref_v<decltype(p_member_const_fn1)>);

	//has new
	AssertFalse(has_new_v<int>);
	AssertTrue(has_new_v< detail2::TestHasDelete2>);
	AssertTrue((has_new_v<detail2::TestHasDelete2, void*>));
	AssertFalse((has_new_v<detail2::TestHasDelete2, int&>));

	//has delete
	AssertFalse(has_delete_v<int>);
	AssertTrue(has_delete_v<detail2::TestHasDelete2>);
	AssertTrue((has_delete_v<detail2::TestHasDelete2, size_t>));
	AssertTrue((has_array_delete_v<detail2::TestHasDelete2, size_t>));
	AssertFalse((has_array_delete_v<detail2::TestHasDelete2, int>));

	//has indexer
	struct TestIndex { int operator[](int) {} };
	struct TestTable { int operator[](const char*) {} };
	AssertTrue((has_index_v<int[2], int, int>));
	AssertTrue((has_index_v<TestIndex, int, int>));
	AssertFalse((has_index_v<TestIndex, int, float>));
	AssertTrue((has_index_v<TestTable, int, const char*>));
	AssertFalse((has_index_v<TestTable, float, const char*>));

	//has invoker
	struct TestInvoker { int operator()(int, int); };
	AssertTrue((has_invoker_v<TestInvoker, int, int, int>));
	AssertFalse((has_invoker_v<TestInvoker, int, int, float>));

	//has arrow
	struct TestArrow { void* operator->() { return 0; } };
	AssertTrue((has_arrow_v<TestArrow, void*>));
	AssertFalse((has_arrow_v<TestArrow, char*>));
	AssertFalse((has_arrow_v<int, int>));

	//has inner type
	AssertTrue(has_inner_type_v < false_>);
	AssertTrue(has_inner_value_type_v< false_>);
	AssertFalse(has_inner_type_v< int>);


	

	output_test_info();
	getchar();
	return 0;
}

#define PrintIndex(T) printf("index %d \n", trait_fn_indexer<T>::value)
#define PrintDispatch(T) printf("disp %d \n", dispatch_< is_void<T>, is_integer<T>, is_pointer<T>, is_const<T> >::value)
#define OutputTypename(v) printf("%s\n", typeid(v).name())



void output_test_info()
{
	
	
	//using T = compatible_type<int, float, long long, long double, A>::type;
	//printf("%s\n", typeid(T).name());
	//OutputTypename( compatible_type<int, float>::type );
	return;
	TestMemberClass c;
	TestMemberClass& d = c;
	const TestMemberClass e{};
	void (TestMemberClass::*p_member_const_fn1)() const = &TestMemberClass::member_const_fn;
	void (TestMemberClass::*p_member_const_fn2)() volatile = &TestMemberClass::member_const_fn;
	void (TestMemberClass::*p_member_const_fn3)() const volatile = &TestMemberClass::member_const_fn;
	void (TestMemberClass::*p_member_const_fn4)() = &TestMemberClass::member_const_fn;
	void (TestMemberClass::*p_member_ref_fn1)() && = &TestMemberClass::member_ref_fn;
	void (TestMemberClass::*p_member_ref_fn2)() & = &TestMemberClass::member_ref_fn;
	void (TestMemberClass::*p_member_ref_fn3)() const& = &TestMemberClass::member_ref_fn;
	constexpr void (TestMemberClass::*p_member_noexcept_fn3)() noexcept = &TestMemberClass::member_noexcept_fn;
	(c.*p_member_const_fn1)();
	
	(c.*p_member_const_fn2)();
	(c.*p_member_const_fn3)();
	(c.*p_member_const_fn4)();
	(std::move(c).*p_member_ref_fn1)();
	(c.*p_member_ref_fn2)();
	(c.*p_member_ref_fn3)();

	(d.*p_member_const_fn1)();
	(d.*p_member_const_fn2)();
	(d.*p_member_const_fn3)();
	(d.*p_member_const_fn4)();
	(std::move(d).*p_member_ref_fn1)();
	(d.*p_member_ref_fn2)();
	(d.*p_member_ref_fn3)();

	(e.*p_member_const_fn1)();
	(e.*p_member_const_fn3)();
	//(e.*p_member_const_fn2)();
	//(e.*p_member_const_fn4)(); //const call non-const function cause error
	//(e.*p_member_ref_fn1)();   //&& 函数只能为右值
	//(e.*p_member_ref_fn2)();   
	(e.*p_member_ref_fn3)();
	OutputTypename(decltype(&TestMemberClass::static_fn));
	printf("%s\n", typeid(decltype(&TestMemberClass::member_fn)).name());
	printf("%s\n", typeid(decltype(&TestMemberClass::member_fn) const).name());
	printf("%s\n", typeid(decltype(&TestMemberClass::public_obj)).name());
	printf("%s\n", typeid(decltype(&TestMemberClass::static_obj)).name());
	printf("%s\n", typeid(decltype(&TestMemberClass::member_constexpr_fn)).name());
	printf("%s\n", typeid(decltype(p_member_const_fn1)).name());
	printf("%s\n", typeid(decltype(p_member_const_fn2)).name());
	printf("%s\n", typeid(decltype(p_member_ref_fn1)).name());
	printf("%s\n", typeid(decltype(p_member_ref_fn2)).name());
	printf("%s\n", typeid(decltype(p_member_ref_fn3)).name());
	printf("%s\n", typeid(decltype(p_member_noexcept_fn3)).name());
	printf("%s\n", typeid(decltype(&varg_fn)).name());
	printf("%s\n", typeid(decltype(&normal_fn)).name());
	printf("%s\n", typeid(decltype(&TestMemberClass::member_varg_fn)).name());
	printf("%s\n", typeid(decltype(p_member_noexcept_fn3)).name());

	printf("/// 1. varg function\n");
	OutputTypename(decltype(varg_cd_fn));
	OutputTypename(decltype(*varg_cd_fn));
	
	OutputTypename(decltype(varg_sc_fn));
	OutputTypename(decltype(varg_fc_fn));
	//OutputTypename(decltype(varg_vc_fn));
	OutputTypename(decltype(varg_fn));

	printf("/// 1. varg function\n");
	void(*const normal_const)() = normal_cd_fn;
	void(*volatile normal_volatile)() = normal_cd_fn;
	void(*const volatile normal_cv)() = normal_cd_fn;
	OutputTypename(decltype(normal_cd_fn));
	OutputTypename(decltype(normal_sc_fn));
	OutputTypename(decltype(normal_fc_fn));
	OutputTypename(decltype(normal_vc_fn));
	OutputTypename(decltype(normal_fn));
	OutputTypename(decltype(normal_const));
	OutputTypename(decltype(normal_volatile));
	OutputTypename(decltype(normal_cv));
	OutputTypename(decltype(normal_fn) const);
	OutputTypename(decltype(normal_fn) volatile);
	OutputTypename(decltype(normal_fn) const volatile);
}



