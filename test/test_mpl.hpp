#include<iostream>
#include<vector>
using namespace std;

#include"print_type.h"
#include<core/mpl/int_.h>
#include<core/mpl/logic_and.h>
#include<core/mpl/logic_or.h>
#include<core/mpl/bool_.h>
#include<core/mpl/max.h>
#include<core/mpl/min.h>
#include<core/mpl/mul.h>
#include<core/mpl/add.h>
#include<core/mpl/bit_or.h>
#include<core/mpl/bit_and.h>

#include<core/mpl/enable_if.h>
#include<core/mpl/math/bit_compose.h>
#include<core/mpl/math/bit_sequence.h>

#include<core/mpl/type_traits/is_integral.h>
#include<core/mpl/meta_function/apply.h>
#include<core/mpl/meta_function/lambda.h>
#include<core/mpl/meta_function/placeholder.h>
#include<core/mpl/type_traits/add_pointer.h>

#include<core/mpl/type_traits/has_inner_type.h>
#include<core/mpl/container/vector_.h>
#include<core/mpl/container/vector_insert.h>
#include<core/mpl/container/vector_at.h>
#include<core/mpl/container/vector_erase.h>
#include<core/mpl/container/vector_fore_part.h>
#include<core/mpl/container/vector_post_part.h>
#include<core/mpl/container/vector_append.h>
#include<core/mpl/container/vector_back_op.h>
#include<core/mpl/container/vector_front_op.h>
#include<core/mpl/container/set_.h>

#include<core/mpl/iterator/begin.h>
#include<core/mpl/iterator/deref.h>
#include<core/mpl/iterator/end.h>

#include"print_type.h"
using namespace Aurora3D::mpl;


struct HasType { typedef int type; };
struct NoType{};

template<typename T>
struct TestInnerType {
	//typedef typename T::type1 type1;     //cause error
	typedef typename T::type type;

	template<typename U>
	struct TestInnerFunc
	{
		typedef typename T::type2 type2;   //not cause error
	};
};

template<char... a>
struct String {};

inline auto Functest(int a)->decltype(a)
{
	if (a > 1)
		return Functest(a - 1)*a;
	else return a;
}




inline void TestMpl()
{
	auto a = 0b111;

	String<'fdas'>{};
	[](auto a) {
		
	};
	cout << "==== Test type ====" << endl;
	typedef Vector_<int> vector1;
	typedef typename VectorPushBack<vector1, char>::type vector2;
	typedef typename VectorPushFront<vector2, short>::type vector3;
	typedef typename VectorInsert<vector3, 1, unsigned short>::type vector4;
	typedef typename VectorInsert<vector4, 3, unsigned int>::type vector5;
	typedef typename VectorInsert<vector5, 5, unsigned char>::type vector6;
	typedef typename VectorInsert<vector6, 6, float>::type vector7;
	typedef typename VectorPopBack<vector7>::type vector8;
	typedef typename VectorPopFront<vector8>::type vector9;
	typedef typename VectorErase<vector9, 3>::type vector10;
	typedef typename VectorAt<Vector_<int,char>, 0>::type elem1;
	typedef typename VectorForePart<vector10, 2>::type vector11;
	typedef typename VectorPostPart<vector10, 2>::type vector12;
	typedef typename VectorAppend<vector11, vector12>::type vector13;
	typedef Type<VectorInsert<vector13, 0, float, double, long long>> vector14;
	typedef VectorReverseT<vector14> vector15;

	NormalTypeName<vector1>{}("original-");
	NormalTypeName<vector2>{}("push back char-");
	NormalTypeName<vector3>{}("push front short-");
	NormalTypeName<vector4>{}("insert 1 ushort-");
	NormalTypeName<vector5>{}("insert 3 uint-");
	NormalTypeName<vector6>{}("insert 5 uchar-");
	NormalTypeName<vector7>{}("insert 6 float-");
	NormalTypeName<vector8>{}("pop back-");
	NormalTypeName<vector9>{}("pop front-");
	NormalTypeName<vector10>{}("erase at 3-");
	NormalTypeName<vector11>{}("[0,2)  part-");
	NormalTypeName<vector12>{}("[2,end) part-");
	NormalTypeName<vector13>{}("append vector11 and vector12");
	NormalTypeName<vector14>{}("inserted 3 elem at 0-");
	NormalTypeName<vector15>{}("reverse-");
	NormalTypeName<elem1>{}("at 1-");
	NormalTypeName<Type<Deref<Begin<vector15>>>>{}("iterator begin-");
	//NormalTypeName<Type<Deref<End<vector15>>>>{}("iterator begin-");


	cout << "==== HasInnerType ====" << endl;
	cout << HasInnerType<HasType>::value << endl;
	cout << HasInnerType<NoType>::value << endl;


	cout << "==== Placeholder Traits ====" << endl;
	cout << IsPlaceholder<_1>::value << endl;
	cout << IsPlaceholder<_2>::value << endl;
	cout << IsPlaceholder<Arg<17>>::value << endl;
	cout << ContainNPlaceholder<_>::value << endl;
	cout << ContainNPlaceholder<_1, _2>::value << endl;
	cout << ContainNPlaceholder<_1, _2, _>::value << endl << endl;;

	cout << "==== AddApply ====" << endl;
	cout << Apply< AddApply<Add>, Int_<5>, Int_<3> >::type::value << endl;

	cout << "==== Lambda ====" << endl;
	//2
	typedef  Apply<Lambda<Add<_1>>, int1_, int2_> choose21;
	typedef  Apply<Lambda<Add<_2>>, int1_, int2_> choose22;
	//1+2
	typedef  Apply<Lambda<Add<_1, _2>>, int1_, int2_> first21;
	//1+2
	typedef  Apply<Lambda<Add<_, _>>, int1_, int2_> first22;
	//1+2
	typedef  Apply<Lambda<Add<__>>, int1_, int2_> first23;
	//(1+2)+(1*2*3*4) =27
	typedef  Apply<Lambda<Add<Add<_1, _2>, Mul<_1, _2, _3, _>>>, int1_, int2_, int3_, int4_> second21;
	// 1+2+1*2*5 = 13
	typedef  Apply<Lambda<Add<Add<_, _>, Mul<__>>>, int1_, int2_, int5_> second22;
	// 1+2 + 1*2
	typedef  Apply<Lambda<Add<Add<__>, Mul<__>>>, int1_, int2_> second23;
	//1+(2+(3+4+(1+2+3+4+5+6))) = 31
	typedef Apply<Lambda<Add<_1, Add<_2, Add<_3, Add<_4, Add<__>>>>>>, int1_, int2_, int3_, int4_, int5_, int6_> six21;
	//replace vector<_> with type
	typedef typename Apply<Lambda<std::vector<_3>>, int, char, short>::type  int_of_vector;

	cout << choose21::type::value << endl;
	cout << choose22::type::value << endl;
	cout << first21::type::value << endl;
	cout << first22::type::value << endl;
	cout << first23::type::value << endl;
	cout << second21::type::value << endl;
	cout << second22::type::value << endl;
	cout << second23::type::value << endl;
	cout << six21::type::value << endl;
	NormalTypeName<int_of_vector>{}("");

	cout << "==== Or/And ====" << endl;
	cout << "value£º" << Or<False_>::value << endl;
	cout << "value£º" << Or<False_, False_>::value << endl;
	cout << "value£º" << Or<False_, True_>::value << endl;
	cout << "value£º" << Or<False_, False_, False_>::value << endl;
	cout << "value£º" << Or<False_, False_, True_>::value << endl<<endl;
	cout << "value£º" << And<True_>::value << endl;
	cout << "value£º" << And<True_, False_>::value << endl;
	cout << "value£º" << And<True_, True_>::value << endl;
	cout << "value£º" << And<False_, True_>::value << endl;
	cout << "value£º" << And<True_, True_, True_>::value << endl;
	cout << "value£º" << And<True_, False_, True_>::value << endl;
	cout << "==== Max/Min ====" << endl;
	cout << "value:" << MaxV( 1, 2, 3, 2, 1) << endl;
	cout << "value:" << Max<Int_<1>, Int_<2>, Int_<3>, Int_<2>, Int_<1> >::value << endl;
	cout << "value:" << MinV( 4u, 2, 3, 2, 4)<< endl;
	cout << "value:" << Min<Int_<3>, Int_<2>, Int_<3>, Int_<2>, Int_<1> >::value << endl;
	cout << "==== BitCompose ====" << endl;
	//cout << IntBitCompose<7,0,3,5>::value << endl;
	//cout << BitCompose< Int_<0>, Int_<1>, Int_<2>, Int_<3> >::value << endl;

	cout << "==== BitSequence ====" << endl;
	/*cout << BitSequence<1101>::value << endl;
	cout << BitSequence<111>::value << endl;
	cout << BitSequence<11>::value << endl;
	cout << BitSequence<1>::value << endl;*/

	cout << "==== BitOr/BitAnd ====" << endl;
	cout << BitOr< Int_<1>, Int_<2>, Int_<4>, Int_<8> >::value << endl;
	cout << BitAnd< Int_<7>, Int_<15>, Int_<1>>::value << endl;


	cout << "==== If ====" << endl;
	cout << If<IsIntegral<int>, True_, False_>::type::value << endl;
	cout << If<IsIntegral<float>, True_, False_>::type::value << endl;
	cout << BoolIf<false, True_, False_>::type::value << endl;
	cout << BoolIf<true, True_, False_>::type::value << endl << endl;

	cout << DeriveIf<IsIntegral<int>, True_, False_>::value << endl;
	cout << DeriveIf<IsIntegral<float>, True_, False_>::value << endl;
	cout << BoolDeriveIf<false, True_, False_>::value << endl;
	cout << BoolDeriveIf<true, True_, False_>::value << endl;
}