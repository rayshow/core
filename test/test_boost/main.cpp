
#include<cstdio>

#include<boost/proto/proto.hpp>
#include<boost/any.hpp>
#include<boost/lockfree/queue.hpp>
#include<boost/mpl/filter_view.hpp>
#include<boost/smart_ptr/make_shared.hpp>
#include<boost/phoenix.hpp>

using namespace boost;

proto::terminal< std::ostream&>::type cout_ = { std::cout };
proto::literal<int> int_ = 1;

template<typename Expr>
void evaluate(Expr const& expr)
{
	proto::default_context ctx;
	proto::eval(expr, ctx);
}


typedef int const&  iref;
int main()
{
	make_shared<int,int>(1, 2);
	evaluate(cout_ << "hello" << "," << "world");
	evaluate(int_ + 1);
	getchar();
	return 0;
}