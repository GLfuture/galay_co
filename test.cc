// cppco.cpp: 定义应用程序的入口点。
//

#include "galay_co.h"
#include "galay_co_function.h"
#include <string.h>
#include <signal.h>
using namespace std;

Coroutine<int> recv_and_send(int fd)
{
	char buffer[512];
	size_t len;
	Coroutine<int> rco = Co_Function::co_recv(fd, buffer, 512);
	Coroutine<int> wco = Co_Function::co_send(fd,buffer,&len);
	while (1)
	{
		if(scheduler->is_stop())
		{
			co_return std::move(1);
		}
		// if(rco.promise().result() == -1) continue;
		// std::string msg1(buffer,rco.promise().result());
		// std::cout<<msg1<<std::endl;
		co_yield 1;
		if(scheduler->get_cur_event()->events & EPOLLIN)
		{
			rco.resume();
			if(rco.done()) break;
			len = rco.promise().result() ;
			std::string msg(buffer, rco.promise().result());
			std::cout << msg << std::endl;
			scheduler->mod_epoll(fd, EPOLLOUT);
		}else if(scheduler->get_cur_event()->events & EPOLLOUT)
		{
			wco.resume();
			if(wco.done()) break;
			std::cout<< wco.promise().result() <<" " << buffer<<'\n';
			scheduler->mod_epoll(fd, EPOLLIN);
			memset(buffer,0,512);
		}
		
	}
	co_return -1;
}

Coroutine<int> get_conn(int fd)
{
	while (1)
	{
		sockaddr sin;
		socklen_t len = sizeof(sockaddr);
		Coroutine<int> aco = Co_Function::co_accept(fd, &sin, &len);
		if(aco.promise().get_status() == SUSPEND) co_yield 1;
		aco.resume();
		if(aco.promise().result() <= 0) continue; 
		Coroutine<int>* rwco = new Coroutine<int>(recv_and_send(aco.promise().result()));
		scheduler->add_coroutine(aco.promise().result(),rwco);
	}
}

void signal_handle(int sing)
{
	scheduler->stop();
	delete scheduler;
	exit(-1);
}

int main()
{
	signal(SIGINT,signal_handle);
	int fd = Co_Function::co_socket(AF_INET,SOCK_STREAM,0);
	
	Co_Function::co_bind(AF_INET,fd,9999,INADDR_ANY);
	Co_Function::co_listen(fd,10);
	scheduler->add_epoll(fd,EPOLLIN);
	Coroutine<int>* server = new Coroutine<int>(get_conn(fd));
	scheduler->add_coroutine(fd,server);
	scheduler->run();
	
	return 0;
}
