#include <iostream>
#include "galay_co_scheduler.h"
#include "galay_co_function.h"

Co_Net_Scheduler<void>* scheduler = Co_Net_Scheduler<void>::GetInstance("system");

Coroutine<void> Connect_Baidu(int fd)
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


int main()
{
    int fd = Co_Function::co_socket(AF_INET,SOCK_STREAM);
    Coroutine<void>* co = new Coroutine<void>(Connect_Baidu(fd));
    scheduler->add_coroutine(fd,co);
    scheduler->add_epoll(fd,EPOLLIN | EPOLLOUT);
    scheduler->run();

    return 0;
}