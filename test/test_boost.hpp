#pragma once

#include<boost/mpl/lambda.hpp>
#include<core/mpl/meta_function/lambda.h>
#include<boost/metaparse/string.hpp>
#include<boost/type_index.hpp>
#include<boost/variant.hpp>
#include<boost/variant/variant.hpp>
#define A 10


struct B {};

inline void TestBoost()
{
	using namespace boost::mpl;

	

	std::cout << boost::typeindex::type_id_with_cvr<BOOST_METAPARSE_STRING("11+2")>() << endl;
	cout << typeid( boost::metaparse::string<'f', 'd', 'a', 's'>() ).name() << endl;

	return;
}