

#include "galay_co.h"
#include "galay_co_function.h"
#include <string.h>
#include <signal.h>
using namespace std;

Co_Net_Scheduler<int>* scheduler = Co_Net_Scheduler<int>::GetInstance("system");

MainCoroutine<int> recv_and_send(int fd)
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
		co_yield 1;
		if(scheduler->get_cur_event()->events & EPOLLIN)
		{
			rco.resume();
			if(rco.done()) 
			{
				scheduler->del_epoll(fd,EPOLLIN);
				break;
			}
			len = rco.promise().result() ;
			if(len != -1){
				std::string msg(buffer, rco.promise().result());
				scheduler->mod_epoll(fd, EPOLLOUT);
			}
		}else if(scheduler->get_cur_event()->events & EPOLLOUT)
		{
			wco.resume();
			if(wco.done()) {	
				scheduler->del_epoll(fd,EPOLLOUT);
				break;
			}
			if(wco.promise().result() != -1) scheduler->mod_epoll(fd, EPOLLIN);
			memset(buffer,0,512);
		}
		
	}
	co_return -1;
}

MainCoroutine<int> get_conn(int fd)
{
	sockaddr sin;
	socklen_t len = sizeof(sockaddr);
	Coroutine<int> aco = Co_Function::co_accept(fd, &sin, &len);
	while (1)
	{
		if(aco.promise().result() == SUSPEND) co_yield 1;
		aco.resume();
		if(aco.promise().result()!= SUSPEND)
		{
			std::cout << aco.promise().result() << std::endl;
			MainCoroutine<int> *rwco = new MainCoroutine<int>(recv_and_send(aco.promise().result()));
			scheduler->add_coroutine(aco.promise().result(), rwco);
			scheduler->add_epoll(aco.promise().result(), EPOLLIN);
		}
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
	int fd = Co_Function::co_socket(AF_INET,SOCK_STREAM);
	Co_Function::co_bind(AF_INET,fd,9999,INADDR_ANY);
	Co_Function::co_listen(fd,10);
	scheduler->add_epoll(fd,EPOLLIN);
	MainCoroutine<int>* server = new MainCoroutine<int>(get_conn(fd));
	scheduler->add_coroutine(fd,server);
	scheduler->run();

	return 0;
}
