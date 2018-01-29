
#include<ccdk/type.h>
#include<ccdk/string/string_literial.h>
#include<ccdk/mpl/function/partial.h>
#include<cstdio>

#include<boost/proto/proto.hpp>
#include<boost/any.hpp>
#include<boost/lockfree/queue.hpp>

using namespace ccdk::mpl::fn;
using namespace ccdk;


using namespace boost;

proto::terminal< std::ostream&>::type cout_ = { std::cout };

template<typename Expr>
void evaluate(Expr const& expr)
{
	proto::default_context ctx;
	proto::eval(expr, ctx);
}

int main()
{
	evaluate(cout_ << "hello" << "," << "world");
	return 0;
}