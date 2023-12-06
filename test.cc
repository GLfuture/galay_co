// cppco.cpp: 定义应用程序的入口点。
//

#include "galay_co.h"
#include "galay_co_function.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

using namespace std;

Co_Net_Scheduler<Net_Result::Ptr>* scheduler = Co_Net_Scheduler<Net_Result::Ptr>::GetInstance("system");

Coroutine<Net_Result::Ptr> recv_and_send(int fd)
{
	char buffer[512];
	while (1)
	{
		memset(buffer,0,512);
		std::shared_ptr<Net_Result> res = std::make_shared<Net_Result>(1);
		co_yield res;

		int len = recv(fd, buffer, 512, 0);
		if (len == -1) continue;
		else if(len == 0 ){
			scheduler->del_corotine(fd);
			co_return std::move(res);
		}

		len = send(fd, buffer, 512, 0);
	}
	
}

Coroutine<Net_Result::Ptr> get_conn(int fd)
{
	while (1)
	{
		sockaddr sin;
		socklen_t len;
		co_await Awaiter();
		int sockfd = accept(fd, &sin, &len);
		Net_Result::Ptr res = std::make_shared<Net_Result>(sockfd);
		if(sockfd > 0){
			epoll_event ev;
			ev.data.fd = sockfd;
			ev.events = EPOLLIN;
			epoll_ctl(scheduler->get_epoll_fd(), EPOLL_CTL_ADD, sockfd, &ev);
			Coroutine<Net_Result::Ptr> func = recv_and_send(sockfd);
			scheduler->add_coroutine(sockfd,&func);
			if(func.promise().get_status() == SUSPEND) co_yield std::move(res);
			std::cout<<"add: "<< sockfd << " "<< &func<<std::endl;
		}else{
			continue;
		}
		
		co_yield std::move(res);
	}
}


int main()
{
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
	Coroutine<Net_Result::Ptr> server = get_conn(fd);
	scheduler->add_coroutine(fd,&server);
	scheduler->run();
	
	return 0;
}
