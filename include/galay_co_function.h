#ifndef __GALAY_CO_FUNCTION_H__
#define __GALAY_CO_FUNCTION_H__

#include "galay_co_scheduler.h"
#include "galay_co_result.h"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

class Co_Function
{
public:
	static int co_socket(int domain,int type,int protocol)
	{
        int fd = socket(domain, type, protocol);
        if (fd == -1)
        {
            return -1;
        }
        int ret = fcntl(fd, F_SETFL, O_NONBLOCK);
        if (ret == -1)
        {
            close(fd);
            return -1;
        }
        int option = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&option, sizeof(option));
        return fd;
    }

    static Coroutine<int> co_accept(int fd , sockaddr* addr , socklen_t* len)
    {
        int sockfd;
        while (1)
        {
            sockfd = accept(fd,addr,len);
            if(sockfd > 0)
            {
                int ret = fcntl(sockfd, F_SETFL, O_NONBLOCK);
                if (ret == -1)
                {
                    close(sockfd);
                    co_yield ret;
                }
                else
                {
                    epoll_event ev;
                    ev.data.fd = sockfd;
                    ev.events = EPOLLIN;
                    scheduler->add_epoll(sockfd, ev);
                    std::cout<<sockfd<<std::endl;
                    co_yield sockfd;
                }
            }else{
                co_yield sockfd;
            }
        }
        co_return std::move(sockfd);
    }

    static Coroutine<int> co_recv(int fd, void *buf, size_t n, int flags)
    {
        int len;
        while(1)
        {
            len = recv(fd, buf, n, flags);
            if(len == 0 || (len == -1 && !(errno == EWOULDBLOCK || errno == EINTR || errno == EAGAIN  ))){
                epoll_event ev;
                ev.data.fd = fd;
                ev.events = EPOLLIN;
                scheduler->del_epoll(fd,ev);
                close(fd);
                break;
            }
            co_yield len;
        }
        co_return std::move(len);
    }

    static Coroutine<int> co_send(int fd, void *buf, size_t n, int flags)
    {
        int len;
        while (1)
        {
            len = send(fd, buf, n, flags);
            if(len == -1 && !(errno == EWOULDBLOCK || errno == EINTR || errno == EAGAIN )){
                epoll_event ev;
                ev.data.fd = fd;
                ev.events = EPOLLIN;
                scheduler->del_epoll(fd,ev);
                close(fd);
                break;
            }
            co_yield len;
        }
        co_return std::move(len);
    }

    
};



#endif // !__GALAY_CO_FUNCTION_H__
