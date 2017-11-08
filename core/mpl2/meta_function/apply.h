#pragma once

#include<core/mpl/int_.h>
namespace Aurora3D
{
	namespace mpl
	{
		//for template mata function
		template<template<typename T, typenameprefix,... TArgs> typename Fn>
		struct AddApply
		{
			template<typename N, typenameprefix,... NArgs>
			struct Apply :public Fn<N, NArgsprefix,...> 
			{
				//keep original Fn
				typedef Fn<N, NArgsprefix,...>  wrapper_type;
			};
		};

		template<typename Fn, typenameprefix,... Args> 
		struct Apply:public Fn::template Apply<Argsprefix,...> {};

		template<typename Fn, typenameprefix,... Args>
		struct CallApply :public Fn::template Apply<Argsprefix,...> {};

		template<typename T>
		struct IdentityApply
		{
			template<typename N, typenameprefix,... NArgs>
			struct Apply
			{
				typedef T type;
			};
		};
	}
}