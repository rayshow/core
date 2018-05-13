
#include<ccdk/container/adapter/queue.h>
#include<ccdk/container/adapter/stack.h>
#include<stdio.h>
#include<string>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::ct;

int main()
{
	DebugNewTitle("queue");
	{
		queue<int> que{ { 1,2,3 } };
		for (uint32 i = 10; i > 0; --i) {
			que.push(i);
		}
		while (!que.empty()) {
			DebugValueIt(que.front());
			que.pop();
		}
	}
	DebugNewTitle("cached_queue");
	{
		cached_queue<int,10> que{ { 1,2,3 } };
		for (uint32 i = 10; i > 0; --i) {
			que.push(i);
		}
		while (!que.empty()) {
			DebugValueIt(que.front());
			que.pop();
		}
	}
	DebugNewTitle("stack");
	{
		stack<int> stack{ {1,2,3,4} };
		for (uint32 i = 10; i > 0; --i) {
			stack.push(i);
		}
		while (!stack.empty()) {
			DebugValueIt(stack.top());
			stack.pop();
		}
	} 
	DebugNewTitle("local stack");
	{
		local_stack<int, 10> stack{ {1, 2, 3, 4, 5}};
		while (!stack.empty()) {
			DebugValueIt(stack.top());
			stack.pop();
		}
	}
	getchar();
	return 0;
}