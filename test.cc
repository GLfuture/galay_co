// cppco.cpp: 定义应用程序的入口点。
//

#include "galay_co.h"
#include "galay_co_function.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
using namespace std;

Coroutine<int> recv_and_send(int fd)
{
	char buffer[512];
	while (1)
	{
		memset(buffer,0,512);
		if(scheduler->is_stop())
		{
			co_return std::move(1);
		}
		Coroutine<int> rco = Co_Function::co_recv(fd, buffer, 512, 0);
		// if(rco.promise().result() == -1) continue;
		// std::string msg1(buffer,rco.promise().result());
		// std::cout<<msg1<<std::endl;
		if(rco.promise().result() == -1) {
			co_yield 1;
			rco.resume();
		}
		if(rco.done()){
			break;
		}
		std::string msg(buffer,rco.promise().result());
		std::cout<<msg<<std::endl;
		epoll_event ev;
		ev.data.fd = fd;
		ev.events = EPOLLOUT;
		scheduler->mod_epoll(fd,ev);
		Coroutine<int> wco = Co_Function::co_send(fd,buffer,rco.promise().result(),0);
		if(wco.promise().result() == -1) {
			co_yield 1;
			wco.resume();
		}
		if(wco.done()) break;
		ev.events = EPOLLIN;
		scheduler->mod_epoll(fd,ev);
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
		std::cout<<"ready to resume\n";
		aco.resume();
		std::cout<<"resume success\n";
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
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(9999);
	saddr.sin_addr.s_addr = INADDR_ANY;
	bind(fd,(sockaddr*)&saddr,sizeof(sockaddr));
	listen(fd,10);
	epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN;
	epoll_ctl(scheduler->get_epoll_fd(),EPOLL_CTL_ADD,fd,&ev);
	Coroutine<int>* server = new Coroutine<int>(get_conn(fd));
	scheduler->add_coroutine(fd,server);
	scheduler->run();
	
	return 0;
}
