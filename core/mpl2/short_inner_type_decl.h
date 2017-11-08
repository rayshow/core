#pragma once

namespace Aurora3D
{
	namespace mpl
	{
		//some time make template more clear and short
		template<typename T> using Head = typename T::head;
		template<typename T> using Tail = typename T::tail;
		template<typename T> using Type = typename T::type;
		template<typename T> using Base = typename T::base;
		template<typename T> using Next = typename T::next;
		template<typename T> using Back = typename T::back;
		template<typename T> using Front = typename T::front;
		template<typename T> using Prior = typename T::prior;
#define LengthV(T)       (T::length)
#define ValueV(T)        (T::value)
	}
}