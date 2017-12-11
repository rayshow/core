//#include<boost/phoenix.hpp>
#include<boost/proto/proto.hpp>
#include<utility>

#include<ccdk/type.h>

using namespace boost::proto;

terminal< int >::type _int;

template<typename T>
auto eval(T const& t)
{
	default_context ctx;
	return proto::eval(t, ctx);
}

int main()
{
	auto a = _int + 1;
	DebugTypeName< decltype(a) >();
	getchar();
	return 0;
}