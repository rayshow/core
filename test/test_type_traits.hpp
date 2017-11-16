#pragma once

#include<iostream>
#include<vector>
#include<tuple>
#include<string>
#include<new>

// pp lib
#include<core/preprocessor/stringize.h>
#include<core/preprocessor/seq_foreach_item.h>
#include<core/preprocessor/seq_foreach_tuple.h>
#include<core/preprocessor/seq_compose.h>
#include<core/preprocessor/sign.h>

// tt lib
#include<core/type.h>
#include<core/mpl/type_traits/is_void.h>
#include<core/mpl/type_traits/is_lvalue_ref.h>
#include<core/mpl/type_traits/is_rvalue_ref.h>
#include<core/mpl/type_traits/is_integral.h>
#include<core/mpl/type_traits/is_float.h>
#include<core/mpl/type_traits/is_arithmetic.h>
#include<core/mpl/type_traits/is_fundamental.h>
#include<core/mpl/type_traits/is_const.h>
#include<core/mpl/type_traits/is_unsigned.h>
#include<core/mpl/type_traits/is_signed.h>
#include<core/mpl/type_traits/is_ref.h>
#include<core/mpl/type_traits/is_class.h>
#include<core/mpl/type_traits/is_union.h>
#include<core/mpl/type_traits/is_abstract.h>
#include<core/mpl/type_traits/is_function.h>
#include<core/mpl/type_traits/is_member_ptr.h>
#include<core/mpl/type_traits/is_member_fn_ptr.h>
#include<core/mpl/type_traits/is_member_obj_ptr.h>
#include<core/mpl/type_traits/is_pointer.h>
#include<core/mpl/type_traits/is_array.h>
#include<core/mpl/type_traits/is_base_of.h>
#include<core/mpl/type_traits/is_convertible.h>

#include<core/mpl/type_traits/add_lvalue_ref.h>
#include<core/mpl/type_traits/add_rvalue_ref.h>
#include<core/mpl/type_traits/add_const.h>
#include<core/mpl/type_traits/add_volatile.h>
#include<core/mpl/type_traits/add_cv.h>
#include<core/mpl/type_traits/add_top_const.h>
#include<core/mpl/type_traits/add_unsigned.h>
#include<core/mpl/type_traits/add_signed.h>
#include<core/mpl/type_traits/add_pointer.h>
#include<core/mpl/type_traits/remove_cv.h>
#include<core/mpl/type_traits/remove_top_const.h>
#include<core/mpl/type_traits/remove_volatile.h>
#include<core/mpl/type_traits/remove_const.h>
#include<core/mpl/type_traits/remove_ref.h>
#include<core/mpl/type_traits/remove_all_dimension.h>
#include<core/mpl/type_traits/remove_dimension.h>
#include<core/mpl/type_traits/first_template.h>
#include<core/mpl/type_traits/decay.h>
#include<core/mpl/type_traits/compatible_type.h>
#include<core/mpl/type_traits/align_of.h>
#include<core/mpl/type_traits/aligned_storage.h>
#include<core/mpl/type_traits/aligned_max.h>
#include<core/mpl/type_traits/array_dimension.h>
#include<core/mpl/type_traits/array_length.h>
#include<core/mpl/type_traits/has_add.h>
#include<core/mpl/type_traits/has_sub.h>
#include<core/mpl/type_traits/has_div.h>
#include<core/mpl/type_traits/has_mul.h>
#include<core/mpl/type_traits/has_mod.h>
#include<core/mpl/type_traits/has_shift_left.h>
#include<core/mpl/type_traits/has_less.h>
#include<core/mpl/type_traits/has_logic_and.h>
#include<core/mpl/type_traits/has_front_dec.h>
#include<core/mpl/type_traits/has_new.h>
#include<core/mpl/type_traits/has_invoker.h>
#include<core/mpl/type_traits/has_indexer.h>
#include<core/mpl/type_traits/has_refer_arrow.h>
#include<core/mpl/type_traits/has_assigner.h>
#include<core/mpl/type_traits/has_constructor.h>
#include<core/mpl/type_traits/has_destructor.h>


#include"print_type.h"
#include"print_value.h"

using namespace std;
using namespace Aurora3D::mpl;

//union, enum, enum class
union TestUnion { int a; };
enum TestNormalEnum { EnumValue = 0 };
enum class TestEnumClassInt8 :Aurora3D::int8 { value = 1 };
enum class TestEnumClassUint8 :Aurora3D::uint8 { value = 1 };

//class
class TestEmpty {};
class TestFinal final {};
class TestAbstract { public: virtual void Test() = 0; static void TestStatic() {}; };
class TestDrivied :public TestAbstract { public: virtual void Test() override {};  void VPrintf(int,...) {} int data; };
class TestCopyConstructible { public: TestCopyConstructible(const TestCopyConstructible&) {} };
class TestNoCopyConstructible { public: TestNoCopyConstructible(const TestNoCopyConstructible&) = delete; };
class TestMoveConstructible { public: TestMoveConstructible(TestMoveConstructible&&) {} };
class TestNoMoveConstructible { public: TestNoMoveConstructible(TestNoMoveConstructible&&) = delete; };
class MutilConstructible { public: MutilConstructible(int) {} MutilConstructible(int, float) {} };
class NoDefaultConstructible { public: NoDefaultConstructible(int) {} };
class DefaultConstructible { public: DefaultConstructible() {} };
class CompoundConstructible { public: CompoundConstructible() {} };
class TestTrivialConstruct1 { public: TestTrivialConstruct1() :a(0) {} private: int a; };

//function
inline void __stdcall TestStdCall() {}
inline void __fastcall TestFastCall(){}
inline void TestNoExcept() noexcept{}
class TestThis { void ThisCall() {} };


struct TestOperator {

	TestOperator(int, int) {}

	void operator+(int) {}
	float operator+(float) { return 0.0f; }
	void operator&(float) {}
	void operator-=(int a) {}
	void operator<<(int a) {}
	bool operator&&(int a) { return false; }
	bool operator<(int a) { return false; }

	bool operator ()(int a, float b)
	{
		return false;
	}

	void* operator new(size_t size)
	{
		return nullptr;
	}

	void* operator new(size_t size, const nothrow_t&, void* mem)
	{
		cout << "place new" << endl;
		(void)size;
		return mem;
	}

	void* operator new(size_t size, void*)
	{
		return nullptr;
	}

	void* operator new(size_t size,  const nothrow_t&)
	{
		return nullptr;
	}

	void* operator new[](size_t size)
	{
		return nullptr;
	}


	//void operator delete(void* mem)
	//{

	//}

	void operator delete(void* mem, size_t size)
	{
		cout << "size:" << size << endl;
	}

	void operator delete(void* mem, const nothrow_t&)
	{

	}

	int operator [](int)
	{
		return 0;
	}

	int operator->()
	{
		return false;
	}
};

class TestDeriveOperator :public TestOperator
{
public:
	int a;
	TestDeriveOperator():TestOperator(0, 0) {}
};

template<typename T>
struct TestCap
{
	void operator()()
	{
		cout << "normal" << endl;
	}
};

template<typename T>
struct TestCap<const T>
{
	void operator()()
	{
		cout << "const" << endl;
	}
};

template<typename T>
struct TestCap<T[]>
{
	void operator()()
	{
		cout << "vector" << endl;
	}
};



inline void TestTypeTraits()
{
	//test set
#define CLASS_SET       (void,nullptr_t, int,TestUnion,TestNormalEnum, TestEmpty, TestFinal, TestAbstract, TestDrivied, TestCopyConstructible, TestNoCopyConstructible, TestNoMoveConstructible, TestMoveConstructible, NoDefaultConstructible)
#define QUALIFERED_SET  CCDK_PP_COMPOSE_EX( (int, const int, volatile int, const volatile int), (CCDK_PP_NULL, &, &&, *, * const, * volatile, * const volatile, [2], [2][3], []), void)
#define ALL_TYPE_SET    (void, nullptr_t, int, float, void(int), int(*)(char), int*,int[2],int&,int&&, decltype(&TestDrivied::Test), decltype(&TestDrivied::data), TestEnumClassInt8, TestNormalEnum, TestUnion, TestEmpty, TestTrivialConstruct1, TestAbstract, TestDrivied)
#define BINARY_TEST_SET ((void, CCDK_PP_NULL, 0), (nullptr_t, CCDK_PP_NULL, 0), (char*, CCDK_PP_NULL, 0), (int, void, 1), (float, void, 1),(TestOperator, void, 1), (int, float, 1), (float, int, 1), (int, ingore_t, 1), (float, CCDK_PP_NULL, 0),(ingore_t, CCDK_PP_NULL, 0), (TestOperator, CCDK_PP_NULL, 0), (int*, float*, 1))

	//printf  format
#define TEST_1TYPE_VALUE(Type, Templ)         TypeValue<Templ<Type>>{}();
#define TEST_TYPE_CONVERTER(Type, Converter)  TypeName<Converter<Type>>{}();
#define PRINT_HAS_BINARY_OP_VALUE(Right,Ret,NeedComma,Left,Templ)   TypeValue<Templ<Left,Right CCDK_PP_IF_COMMA(NeedComma) Ret>>{}();
#define PRINT_HAS_UNARY_OP_VALUE(Type, Ret, Templ) TypeValue<Templ<Type,Ret>>{}();

	//test case
#define TEST_TYPE_CONVERTER_BATCH(Func)               CCDK_PP_FOREACH_ITEM(TEST_TYPE_CONVERTER, QUALIFERED_SET, Func) cout << endl;
	//CCDK_PP_FOREACH_ITEM(TEST_TYPE_CONVERTER, ALL_TYPE_SET, Func) 
#define TEST_TYPE_CONVERTER_BATCH_SP(Func, Seq)       CCDK_PP_FOREACH_ITEM(TEST_TYPE_CONVERTER, Seq, Func) cout << endl;
#define TEST_TYPE_VALUE_BATCH(Templ, Seq)             CCDK_PP_FOREACH_ITEM(TEST_1TYPE_VALUE, Seq, Templ);    cout << endl;
#define TEST_HAS_OP_VALUE_BATCH(Func, Seq, ...)        CCDK_PP_FOREACH_TUPLE(Func,Seq,__VA_ARGS__)
#define TEST_NORMAL_HAS(Templ)	TypeValue<Templ<float*, int*>>{}(); TypeValue<Templ<float*, int>>{}(); TypeValue<Templ<int, int>>{}(); TypeValue<Templ<void, char>>{}(); TypeValue<Templ<int, void>>{}(); TypeValue<Templ<void*, char>>{}();

	cout << "==== Test Unary/Binary HasOperatorXX ====" << endl;
	/*TEST_HAS_OP_VALUE_BATCH(PRINT_HAS_BINARY_OP_VALUE, BINARY_TEST_SET,TestOperator, HasAdd);
	TEST_NORMAL_HAS(HasAdd);*/
	//TEST_HAS_OP_VALUE_BATCH(PRINT_HAS_BINARY_OP_VALUE, BINARY_TEST_SET, TestOperator, HasSub);
	//TEST_NORMAL_HAS(HasSub);
	//TEST_HAS_OP_VALUE_BATCH(PRINT_HAS_BINARY_OP_VALUE, BINARY_TEST_SET, TestOperator, HasSubAssign);
	//TEST_NORMAL_HAS(HasSubAssign);
	//TEST_HAS_OP_VALUE_BATCH(PRINT_HAS_BINARY_OP_VALUE, BINARY_TEST_SET, TestOperator, HasDiv);
	//TEST_NORMAL_HAS(HasDiv);
	//TEST_HAS_OP_VALUE_BATCH(PRINT_HAS_BINARY_OP_VALUE, BINARY_TEST_SET, TestOperator, HasShiftLeft);
	//TEST_NORMAL_HAS(HasShiftLeft);
	//TEST_HAS_OP_VALUE_BATCH(PRINT_HAS_BINARY_OP_VALUE, BINARY_TEST_SET, TestOperator, HasLess);
	//TEST_NORMAL_HAS(HasLess);

	//TypeValue<HasFrontDec<int, int>>{}();
	//TypeValue<HasFrontDec<int, float>>{}();
	//TypeValue<HasFrontDec<float, ingore_t>>{}();
	//TypeValue<HasFrontDec<char>>{}();
	//TypeValue<HasFrontDec<TestOperator>>{}();

	cout << "==== Test HasNew/HasDelete ====" << endl;
	//cout << HasDefaultNew<TestOperator>::value << endl;
	//cout << HasNothrowNew<TestOperator>::value << endl;
	//cout << HasPlacementNew<TestOperator>::value << endl;
	//cout << HasNothrowPlacementNew<TestOperator>::value << endl;
	//cout << HasDefaultArrayNew<TestOperator>::value << endl;
	//cout << HasNothrowArrayNew<TestOperator>::value << endl;
	//cout << HasPlacementArrayNew<TestOperator>::value << endl;
	//cout << HasNothrowPlacementArrayNew<TestOperator>::value << endl;
	//cout << HasInvokerOperator<TestOperator, bool, int, float>::value << endl;
	//cout << HasInvokerOperator<TestOperator, bool, int, int>::value << endl;
	//cout << HasInvokerOperator<TestOperator, bool, int, TestOperator>::value << endl;
	//cout << typeid(decltype(&TestOperator::operator[])).name() << endl;
	//cout << HasReferArrow<TestOperator, bool>::value << endl;
	//cout << HasReferArrow<TestOperator, int>::value << endl;


	cout << "==== Test top/bottom const ====" << endl;
	/*cout << IsSame<const int &, int const &>::value << endl;
	cout << IsSame<const int &, AddTopConst_t<int&>>::value << endl;
	cout << IsSame<const int &, AddConst_t<int&>>::value << endl;
	cout << IsSame<int& const, AddConst_t<int&>>::value << endl;
	cout << TypeNameHelper<int& const>{}() << endl;

	cout << IsSame<const int *, int const *>::value << endl;
	cout << IsSame<const int *, AddTopConst_t<int*>>::value << endl;
	cout << IsSame<const int *, AddConst_t<int*>>::value << endl;
	cout << IsSame<int * const, AddConst_t<int*>>::value << endl;
	cout << TypeNameHelper<int * const>{}() << endl;

	cout << IsSame<const int *, int * const>::value << endl;
	cout << IsSame<AddConst_t<int *>, int * const>::value << endl;
	cout << IsSame<const int &, int & const>::value << endl;*/

	cout << "==== Test print type name: ====" << endl;
	/*cout << TypeNameHelper<int>{}() << endl;
	cout << TypeNameHelper<const int>{}() << endl;
	cout << TypeNameHelper<volatile int>{}() << endl;
	cout << TypeNameHelper<const volatile int>{}() << endl;
	cout << TypeNameHelper<int&>{}() << endl;
	cout << TypeNameHelper<const int&>{}() << endl;
	cout << TypeNameHelper<volatile int&>{}() << endl;
	cout << TypeNameHelper<const volatile int&>{}() << endl;
	cout << TypeNameHelper<int&&>{}() << endl;
	cout << TypeNameHelper<int*>{}() << endl;
	cout << TypeNameHelper<const int*>{}() << endl;
	cout << TypeNameHelper<volatile int*>{}() << endl;
	cout << TypeNameHelper<const volatile int*>{}() << endl;
	cout << TypeNameHelper<int* const>{}() << endl;
	cout << TypeNameHelper<const int* const>{}() << endl;
	cout << TypeNameHelper<volatile int* const>{}() << endl;
	cout << TypeNameHelper<const volatile int* const>{}() << endl;
	cout << TypeNameHelper<int* volatile>{}() << endl;
	cout << TypeNameHelper<const int* volatile>{}() << endl;
	cout << TypeNameHelper<volatile int* volatile>{}() << endl;
	cout << TypeNameHelper<const volatile int* volatile>{}() << endl;
	cout << TypeNameHelper<int* const volatile>{}() << endl;
	cout << TypeNameHelper<const int* const volatile>{}() << endl;
	cout << TypeNameHelper<volatile int* const volatile>{}() << endl;
	cout << TypeNameHelper<const volatile int* const volatile>{}() << endl;*/

	cout << "==== Test AddConst/RemoveConst ====" << endl;
	//TEST_TYPE_CONVERTER_BATCH(AddConst);
	//TEST_TYPE_CONVERTER_BATCH(RemoveConst);

	cout << "==== Test AddVolatile/RemoveVolatile ====" << endl;
	//TEST_TYPE_CONVERTER_BATCH(AddVolatile);
	//TEST_TYPE_CONVERTER_BATCH(RemoveVolatile);

	cout << "==== Test AddTopConst/RemoveTopConst ====" << endl;
	//TEST_TYPE_CONVERTER_BATCH(AddTopConst);
	//TEST_TYPE_CONVERTER_BATCH(RemoveTopConst);

	cout << "==== Test AddPointer/RemovePointer ====" << endl;
	//TEST_TYPE_CONVERTER_BATCH(AddPointer);
	//TEST_TYPE_CONVERTER_BATCH(RemovePointer);

	cout << "==== Test ConvertConstLRef ====" << endl;
	//TEST_TYPE_CONVERTER_BATCH(ConvertConstLRef);

	cout << "==== Test AddCV/RemoveCV ====" << endl;
	//TEST_TYPE_CONVERTER_BATCH(AddCV);
	//TEST_TYPE_CONVERTER_BATCH(RemoveCV);

	cout << "==== Test AddLValueRef/AddRValueRef/RemoveRef ====" << endl;
	/*TEST_TYPE_CONVERTER_BATCH(AddLValueRef);
	TEST_TYPE_CONVERTER_BATCH(AddRValueRef);
	TEST_TYPE_CONVERTER_BATCH(RemoveRef);*/

	cout << "==== Test UnderlyingType/AddUnsigned/AddSigned ====" << endl;
	//TEST_TYPE_CONVERTER_BATCH_SP(UnderlyingType, (TestNormalEnum, TestEnumClassInt8, TestEnumClassUint8, TestAbstract));
	//TEST_TYPE_CONVERTER_BATCH_SP(AddUnsigned, (unsigned char, char, signed char, short, signed short, unsigned short, long int, TestEnumClassInt8, TestEnumClassUint8, TestNormalEnum));
	//TEST_TYPE_CONVERTER_BATCH_SP(AddSigned, (unsigned char, char, signed char, short, unsigned short, long int, TestEnumClassInt8, TestEnumClassUint8, TestNormalEnum));


	cout << "==== Test RemoveDim/RemoveAllAllDim ====" << endl;
	//TEST_TYPE_CONVERTER_BATCH(RemoveDim);
	//TEST_TYPE_CONVERTER_BATCH(RemoveAllDim);

	cout << "==== Test Decay ====" << endl;
	//TEST_TYPE_CONVERTER_BATCH(Decay);

	cout << "==== Test ArrayDim ====" << endl;
	//TEST_TYPE_VALUE_BATCH(ArrayDim, (int, int[3], const int[4], volatile int[4][5], const volatile int[3][4][5], int[], int[][3]));

	cout << "==== Test ArrayLen ====" << endl;
	//TEST_TYPE_VALUE_BATCH(ArrayLen, (int, int[3], const int[4], volatile int[4][5], const volatile int[3][4][5], int[], int[][3]));

	cout << "==== test AlignOf/AlignedMax/Aligned/Storeage ====" << endl;
	/*TEST_TYPE_VALUE_BATCH(AlignOf, (int, char, short, long long, int[2], int *, long long *, int&&, int&));
	cout << AlignedMax_v(1, char) << endl;
	cout << AlignedMax_v(2, char) << endl;
	cout << AlignedMax_v(4, char) << endl;
	cout << AlignedMax_v(8, char) << endl;
	cout << AlignedMax_v(1, int) << endl;
	cout << AlignedMax_v(1, long long) << endl;
	cout << AlignedMax_v(1, short) << endl;
	cout << AlignedMax_v(1, char[3]) << endl;
	cout << AlignedMax_v(1, char[5]) << endl;
	cout << AlignedMax_v(1, int[4]) << endl;
	cout << AlignedMax_v(1, int[8]) << endl;
	cout << AlignedMax_v(1, int[16]) << endl;
	cout << AlignedMax_v(1, int[32]) << endl;
	cout << AlignedMax_v(1, int[64]) << endl;
	cout << AlignedMax_v(1, int[1], int[2], int[3], int[4]) << endl << endl;;
#define AlignAndSize(T) " Type:"<<#T<<" Size: "<<sizeof(AlignedTypeStorage_t<T>)<<" Align:"<<alignof(AlignedTypeStorage_t<T>)
	struct alignas(16) float4 {  float v[4]; };
	cout << AlignAndSize(char) << endl;
	cout << AlignAndSize(char[2]) << endl;
	cout << AlignAndSize(char[3]) << endl;
	cout << AlignAndSize(char[4]) << endl;
	cout << AlignAndSize(char[5]) << endl;
	cout << AlignAndSize(char[6]) << endl;
	cout << AlignAndSize(char[7]) << endl;
	cout << AlignAndSize(char[8]) << endl;
	cout << AlignAndSize(short) << endl;
	cout << AlignAndSize(int) << endl;
	cout << AlignAndSize(long long ) << endl;
	cout << AlignAndSize(float[4]) << endl;
	cout << AlignAndSize(float[8]) << endl;
	cout << AlignAndSize(float[16]) << endl;
	cout << AlignAndSize(char[128]) << endl;
	cout << AlignAndSize(char[132]) << endl ;
	cout << AlignAndSize(float4) << endl << endl;*/

	cout << "==== Test CompatibleType ====" << endl;
	//class BaseClass {};
	//class DerivedClass :public BaseClass {};
	//cout << TypeNameHelper<CompatibleType_t<BaseClass&, DerivedClass&>>{}() << endl;
	//cout << TypeNameHelper<CompatibleType_t<BaseClass, DerivedClass>>{}() << endl;
	//cout << TypeNameHelper<CompatibleType_t<float, int>>{}() << endl;
	//cout << TypeNameHelper<CompatibleType_t<BaseClass* const, DerivedClass* const>>{}() << endl;

	std::is_trivial<int>::value;
	cout << " test IsConst" << endl;
	//TEST_TYPE_VALUE_BATCH(IsConst, (void, char, const int, volatile const float, double const [2], double const * const, double& const, long * const [2] ))

	cout << " test IsUnsigned/IsSigned" << endl;
	//TEST_TYPE_VALUE_BATCH(IsUnsigned, ( char, unsigned char, signed char, signed short, unsigned short, char16_t, char16_t,  char32_t, unsigned long int));
	//TEST_TYPE_VALUE_BATCH(IsSigned, (char, unsigned char, signed char, signed short, unsigned short, char16_t, char16_t, char32_t, unsigned long int));

	cout << " test IsClass/IsUnion/IsAbstract" << endl;
	//TEST_TYPE_VALUE_BATCH(IsClass, (int, void, nullptr_t, IsConst<int>, TestUnion, TestAbstract));
	//TEST_TYPE_VALUE_BATCH(IsUnion, (int, void, nullptr_t, IsConst<int>, TestUnion, TestAbstract));
	//TEST_TYPE_VALUE_BATCH(IsAbstract, (int, void, nullptr_t, IsConst<int>, TestUnion, TestAbstract));

	cout << " test IsMemberFnPtr/IsMemberPtr/IsMemberObjPtr" << endl;
	//TEST_TYPE_VALUE_BATCH(IsMemberFnPtr, (int,decltype(printf), decltype(TestStdCall),  decltype(&TestDrivied::Test), decltype(&TestDrivied::VPrintf), decltype(&TestDrivied::data), decltype(&TestAbstract::Test)));
	//TEST_TYPE_VALUE_BATCH(IsMemberPtr, (int, decltype(printf), decltype(TestStdCall), decltype(&TestDrivied::Test), decltype(&TestDrivied::VPrintf), decltype(&TestDrivied::data), decltype(&TestAbstract::Test)));
	//TEST_TYPE_VALUE_BATCH(IsMemberObjPtr, (int, decltype(printf), decltype(TestStdCall), decltype(&TestDrivied::Test), decltype(&TestDrivied::VPrintf), decltype(&TestDrivied::data), decltype(&TestAbstract::Test)));

	cout << " test IsBaseOf/IsDeriveFrom" << endl;
	//cout << " is base of value:" << IsBaseOf<int, int>::value << endl;
	//cout << " is base of value:" << IsBaseOf<TestAbstract, TestDrivied>::value << endl;
	//cout << " is base of value:" << IsBaseOf<TestDrivied, TestDrivied>::value << endl;
	//cout << " is derive from value:" << IsDeriveFrom<int, int>::value << endl;
	//cout << " is derive from value:" << IsDeriveFrom<TestDrivied, TestAbstract>::value << endl;
	//cout << " is derive from value:" << IsDeriveFrom<TestDrivied, TestDrivied>::value << endl;

	
	cout << " test IsAssignable/IsConvertible" << endl;
	//cout << " is RValueAssignable:" << IsRValueAssignable<int,int>::value << endl;
	//cout << " is RValueAssignable:" << IsRValueAssignable<int,double>::value << endl;
	//cout << " is RValueAssignable:" << IsRValueAssignable<string, double>::value << endl;
	//cout << " is RValueAssignable:" << IsRValueAssignable<TestAbstract*, TestDrivied*>::value << endl;
	//cout << " is LValueAssignable:" << IsLValueAssignable<int, int>::value << endl;
	//cout << " is LValueAssignable:" << IsLValueAssignable<const int, int>::value << endl;
	//cout << " is LValueAssignable:" << IsLValueAssignable<int, double>::value << endl;
	//cout << " is LValueAssignable:" << IsLValueAssignable<string, double>::value << endl;
	//cout << " is LValueAssignable:" << IsLValueAssignable<TestAbstract*, TestDrivied*>::value << endl;
	//cout << " is LValueAssignable:" << IsLValueAssignable<TestAbstract, TestDrivied>::value << endl;
	//cout << " is Convertible:" << IsConvertible<int, int>::value << endl;
	//cout << " is Convertible:" << IsConvertible<const int, int>::value << endl;
	//cout << " is Convertible:" << IsConvertible<double, int>::value << endl;
	//cout << " is Convertible:" << IsConvertible<double, string>::value << endl;
	//cout << " is Convertible:" << IsConvertible<TestDrivied*, TestAbstract*>::value << endl;
	//cout << " is Convertible:" << IsConvertible<TestDrivied, TestAbstract>::value << endl;

	cout << " test IsFunction" << endl;
	//auto lamda = []() {};
	//TEST_TYPE_VALUE_BATCH(IsFunction, (decltype(printf), decltype(TestTypeTraits), decltype(lamda), decltype(TestStdCall), decltype(TestFastCall), decltype(TestNoExcept)));
	//TEST_TYPE_VALUE_BATCH(std::is_function, (decltype(printf), decltype(TestTypeTraits), decltype(lamda), decltype(TestStdCall), decltype(TestFastCall)));

	cout << " test IsIntegral" << endl;
	//TEST_TYPE_VALUE_BATCH(IsIntegral, (void, char, char&,int&&, char16_t, char32_t, wchar_t, short, int, long long, long, float, double, long double, nullptr_t));

	cout << " test IsFloat" << endl;
	//TEST_TYPE_VALUE_BATCH(IsFloat, (void, char, char16_t, char32_t, wchar_t, short, int, long long, long, const float, double, long double, nullptr_t));

	cout << " test IsArithtic" << endl;
	//TEST_TYPE_VALUE_BATCH(IsArithmetic, (void,int *, float[2], double&, char, const int, volatile long, const volatile int, short, int, long long, long, const float, double, long double, nullptr_t));

	cout << " test IsFundamental" << endl;
	//TEST_TYPE_VALUE_BATCH(IsFundamental, (void, int *, float[2], double&, char, const int, volatile long, const volatile int, short, int, long long, long, const float, double, long double, std::nullptr_t));

	cout << " test IsConstructible" << endl;
	/*TEST_TYPE_VALUE_BATCH(IsCopyConstructible, CLASS_SET);
	TEST_TYPE_VALUE_BATCH(IsMoveConstructible, CLASS_SET);
	TEST_TYPE_VALUE_BATCH(IsDefaultConstrictible, CLASS_SET);
	TEST_TYPE_VALUE_BATCH(IsDestructible, CLASS_SET);*/

	cout << " 5. test IsVoid " << endl;
	//TEST_TYPE_VALUE_BATCH(IsVoid, (void, const void, volatile void, const volatile void, int, char));

	cout << " 7. test IsLValueRef " << endl;
	//TEST_TYPE_VALUE_BATCH(IsLValueRef, (int&&, const int&, volatile int&, const volatile int&, int *&, int& const, int& volatile, int& const volatile));


	cout << " 10. test IsRef " << endl;
	//TEST_TYPE_VALUE_BATCH(IsRef, CCDK_PP_COMPOSE((int, int const, int volatile, int const volatile), (CCDK_PP_NULL, &, &&, *)));

	cout << " test IsPointer" << endl;
	//TEST_TYPE_VALUE_BATCH(IsPointer, (void, void * const, void*&, int **,void(*)(int), decltype(&TestFastCall), decltype(&TestAbstract::TestStatic) ) );
	
	cout << " test IsArray" << endl;
	//typedef int Int2[2];
	//TEST_TYPE_VALUE_BATCH(IsArray, (void, char volatile [], int *[2], int[2], Int2 const, char[4]));

}


