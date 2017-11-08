#pragma once

namespace Aurora3D
{
	namespace mpl
	{
		//capacity = 10
		#define A3D_MPL_VECTOR_CAPACITY 10

		//vector
		template<typenameprefix,... TArgs> struct Vector_;

		//iterator
		template<typename S, int P> struct VectorIterator;

		template<typename S, int P> struct VectorReverseIterator;

		//access vector element
		template<typename S, int Pos> struct VectorAt;

		template<typename S, int Pos>
		using VectorAtT = typename VectorAt<S, Pos>::type;

		//insert at least one element at pos
		template<typename S, int Pos, typename T,typenameprefix,... TArgs>
		struct VectorInsert;

		template<typename S, int Pos, typename T, typenameprefix,... TArgs>
		using VectorInsertT = typename VectorInsert<S, Pos, T, TArgsprefix,...>::type;

		//erase element At Pos
		template<typename S, int Pos> struct VectorEraseAt;

		template<typename S, int Pos> 
		using VectorEraseAtT = typename VectorEraseAt<S, Pos>::type;

		//erase element between [Begin, End)
		template<typename S, int Begin, int End =Begin+1> struct VectorErase;

		template<typename S, int Begin, int End = Begin + 1>
		using VectorEraseT = typename VectorErase<S, Begin, End>::type;

		//generate new vector from V1 V2 elements
		template<typename V1, typename V2> struct VectorAppend;
		template<typename V1, typename V2> 
		using VectorAppendT = typename VectorAppend<V1, V2>::type;

		//generate new vector from S sub elements at [0, pos)
		template<typename S, int Pos> struct VectorForePart;
		template<typename S, int Pos>
		using VectorForePartT = typename VectorForePart<S, Pos>::type;

		//generate new vector from S sub elements at [pos, end)
		template<typename S, int Pos> struct VectorPostPart;
		template<typename S, int Pos>
		using VectorPostPartT = typename VectorPostPart<S, Pos>::type;

		//Get 1th element of Vector_
		template<typename S> struct VectorFront;
		template<typename S> using VectorFrontT = typename VectorFront<S>::type;

		//Get last element of Vector_
		template<typename S> struct VectorBack;
		template<typename S> using VectorBackT = typename VectorBack<S>::type;

		//Push one element front of S
		template<typename S, typename T> struct VectorPushFront;
		template<typename S, typename T> 
		using VectorPushFrontT = typename VectorPushFront<S, T>::type;

		//Push one element back of S
		template<typename S, typename T> struct VectorPushBack;
		template<typename S, typename T>
		using VectorPushBackT = typename VectorPushBack<S, T>::type;

		//Pop front element of S
		template<typename S> struct VectorPopFront;
		template<typename S>
		using VectorPopFrontT = typename VectorPopFront<S>::type;

		//Pop back element of S
		template<typename S> struct VectorPopBack;
		template<typename S>
		using VectorPopBackT = typename VectorPopBack<S>::type;

		//reverse elements of S
		//template<typename S> struct VectorReverse;
		template<typename S>
		using VectorReverseT = typename S::reverse;
	}
}