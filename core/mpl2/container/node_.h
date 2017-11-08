#pragma once

#include<core/mpl/null_.h>
#include<core/mpl/int_.h>
#include<core/mpl/short_inner_type_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
#define NodeEmpty(T) 
		//mid node
		template<typename T, typename N>
		struct Node_
		{
			typedef T type;
			typedef N next;
			static constexpr int length = LengthV(N) + 1;
		};

		//last node
		template<typename T>
		struct Node_<T, Null_>
		{
			typedef T type;
			typedef Null_ next;
			static constexpr int length = 1;
		};

		template<typename Node>
		struct NodeLength:public Int_<LengthV(Node)>{};
		template<> struct NodeLength<Null_> :public Int_<0> {};

		//Node can't be Null_
		template<typename Node, int Length = LengthV(Node) >
		struct NodeLast:public NodeLast< Next<Node>, Length-1>{};

		template<typename Node>
		struct NodeLast<Node, 1>
		{
			typedef TypeT<Node> type;
		};
		template<typename Node, int Length = LengthV(Node)>
		using NodeLastT = typename NodeLast<Node, Length>::type;

		//Node can't be Null_
		template<typename Node>
		struct NodeFirst
		{
			typedef TypeT<Node> type;
		};
		template<typename Node>
		using NodeFirstT = typename NodeFirst<T>::type;

		//Node can't be Null_
		template<typename Node, int Length = LengthV(Node),
			typename PreNode = Null_>
		struct NodeReverse
		{
			typedef Node_<typename Node::type, PreNode> node;
			typedef Type< NodeReverse< Next<Node>, Length - 1, node>> type;
		};

		//last node
		template<typename Node, typename PreNode>
		struct NodeReverse<Node, 1, PreNode>
		{
			//first node of reverse list
			typedef Node_<typename Node::type, PreNode> type;
		};
		template<typename Node, int Length = LengthV(Node)>
		using NodeReverseT = typename NodeReverse<Node, Length>::type;

		//Node1 can't be Null_
		template<typename Node1, typename Node2, int Length = LengthV(Node1)>
		struct NodeAppend
		{
			typedef typename NodeAppend< NextT<Node1>, Node2,
				Length - 1>::type next;
			typedef Node_< TypeT<Node1>, next> type;
		};

		//L1 last node
		template<typename Node1, typename Node2>
		struct NodeAppend<Node1, Node2, 1>
		{
			typedef Node_< TypeT<Node1>, Node2> type;
		};
		template<typename Node1, typename Node2>
		using NodeAppendT = typename NodeAppend<Node1, Node2>::type;
	}
}