#pragma once
#include<iostream>
#include<string>
#include<iomanip>
#include<core/mpl/type_traits/first_template.h>
using namespace std;


template<typename T>
struct TypeNameHelper
{
	string operator()()
	{
		return typeid(T).name();
	}
};

template<typename T>
struct TypeNameHelper< const volatile T>
{
	string operator()()
	{
		return  string{ "const volatile " }+TypeNameHelper<T>{}();
	}
};

template<typename T>
struct TypeNameHelper< const T>
{
	string operator()()
	{
		return  string{"const "}+TypeNameHelper<T>{}();
	}
};

template<typename T>
struct TypeNameHelper<volatile T>
{
	string operator()()
	{
		return string{"volatile "}+TypeNameHelper<T>{}();
	}
};

template<typename T>
struct TypeNameHelper<T&>
{
	string operator()()
	{
		return  string{}+TypeNameHelper<T>{}() + " &";
	}
};

template<typename T>
struct TypeNameHelper<T&&>
{
	string operator()()
	{
		return  string{}+TypeNameHelper<T>{}() + " &&";
	}
};

template<typename T>
struct TypeNameHelper<T*>
{
	string operator()()
	{
		return string{}+TypeNameHelper<T>{}() + " *";
	}
};

template<typename T>
struct TypeNameHelper<T* const>
{
	string operator()()
	{
		return string{}+TypeNameHelper<T>{}() + " * const";
	}
};

template<typename T>
struct TypeNameHelper<T* volatile>
{
	string operator()()
	{
		return string{}+TypeNameHelper<T>{}() + " * volatile";
	}
};

template<typename T>
struct TypeNameHelper<T* const volatile>
{
	string operator()()
	{
		return string{}+TypeNameHelper<T>{}() + " * const volatile";
	}
};




template<typename T>
struct TypeName
{
	void operator()()
	{
		cout<<"      "<< left<<setw(40) <<TypeNameHelper<typename FirstTemplateType<T>::type >{}()<<" ==> " << TypeNameHelper<typename T::type >{}() << endl;
	}
};

template<typename T>
struct NormalTypeName
{
	void operator()(const char* prefix)
	{
		cout << prefix << ":" << endl;
		cout << setw(30) <<"type name:" << TypeNameHelper<T>{}() << endl;
	}
};
