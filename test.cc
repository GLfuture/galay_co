// cppco.cpp: 定义应用程序的入口点。
//

#include "galay_co.h"

using namespace std;


static Awaiter operator co_await(Awaiter&& await)
{
	return await;
}

Coroutine<void> say()
{
	std::cout<<"say....\n";
	co_yield 1;
	std::cout<<"say end\n";
	co_return;
}

Coroutine<int> Get_Int(int a)
{
	int sum = 0;
	Coroutine<void> co = say();
	for (int i = a; i < 20; i++)
	{
		std::cout << "stop.....\n";
		sum += i;
	}
	if(co.promise().get_status() == SUSPEND) co_yield 1;
	co.resume();
	std::cout << "end" << std::endl;
	co_return std::move(sum);
}

int main()
{

	{
		Coroutine<int> co = Get_Int(10);
		if(co.promise().get_status() == SUSPEND) 
		{
			getchar();
			co.resume();
		}
		int res = co.promise().result();
		std::cout << res << std::endl;
	}
	getchar();
	return 0;
}
