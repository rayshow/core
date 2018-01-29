
#include<cstdio>

#include<boost/proto/proto.hpp>
#include<boost/any.hpp>
#include<boost/lockfree/queue.hpp>

using namespace boost;

proto::terminal< std::ostream&>::type cout_ = { std::cout };
proto::literal<int> int_ = 1;

template<typename Expr>
void evaluate(Expr const& expr)
{
	proto::default_context ctx;
	proto::eval(expr, ctx);
}

int main()
{
	evaluate(cout_ << "hello" << "," << "world");
	evaluate(int_ + 1);
	getchar();
	return 0;
}