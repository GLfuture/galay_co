#include <iostream>
#include "galay_co_scheduler.h"
#include "galay_co_function.h"
#include <signal.h>

Co_Net_Scheduler<void>* scheduler = Co_Net_Scheduler<void>::GetInstance("system");

MainCoroutine<void> Connect_Baidu(int fd)
{
    auto co = Co_Function::co_connect(AF_INET,fd,"110.242.68.3",80);
    while (1)
    {
        std::cout<< co.promise().result()<<'\n';
        co_yield 1;
        co.resume();
        if(co.done()) {
            if(co.promise().result() == 0) break;
            else std::cout<<"Connect falied\n";
        }
    }
    scheduler->del_epoll(fd,EPOLLIN | EPOLLOUT);
    std::cout<<"Connect Success\n";
    co_return;
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
    MainCoroutine<void>* co = new MainCoroutine<void>(Connect_Baidu(fd));
    scheduler->add_coroutine(fd,co);
    scheduler->add_epoll(fd,EPOLLIN | EPOLLOUT);
    scheduler->run();

    return 0;
}