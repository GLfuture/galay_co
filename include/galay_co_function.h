#ifndef __GALAY_CO_FUNCTION_H__
#define __GALAY_CO_FUNCTION_H__

#include "galay_co_scheduler.h"
#include "galay_co_result.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

enum Ret_Value{
    SUSPEND = -100,
};


class Co_Function
{
public:
	static int co_socket(int domain,int type)
	{
        int fd = socket(domain, type, 0);
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

    static int co_bind(int domain , int fd , int port , uint32_t addr)
    {
        sockaddr_in saddr;
        saddr.sin_family = domain;
        saddr.sin_port = htons(port);
        saddr.sin_addr.s_addr = htonl(addr);
        return bind( fd , (sockaddr *)&saddr, sizeof(sockaddr));
    }

    static int co_listen(int fd,int backlog)
    {
        return listen(fd, backlog);
    }

    static Coroutine<int> co_accept(int fd , sockaddr* addr , socklen_t* len)
    {
        int sockfd = SUSPEND;
        while (1)
        {
            co_yield sockfd;
            sockfd = accept(fd,addr,len);
            if(sockfd > 0)
            {
                int ret = fcntl(sockfd, F_SETFL, O_NONBLOCK);
                if (ret == -1)
                {
                    close(sockfd);
                    sockfd = SUSPEND;
                }  
            }else{
                sockfd = SUSPEND;
            }
        }
        co_return std::move(sockfd);
    }

    static Coroutine<int> co_recv(int fd, void *buf, size_t n)
    {
        int len = SUSPEND;
        while(1)
        {
            co_yield len;
            len = recv(fd, buf, n, 0);
            if(len == 0 || (len == -1 && !(errno == EWOULDBLOCK || errno == EINTR || errno == EAGAIN  ))){
                close(fd);
                break;
            }
        }
        co_return std::move(len);
    }

    static Coroutine<int> co_send(int fd, void *buf, size_t* n)
    {
        int len = SUSPEND;
        while (1)
        {
            co_yield len;
            len = send(fd, buf, *n, 0);
            if(len == -1 && !(errno == EWOULDBLOCK || errno == EINTR || errno == EAGAIN )){
                close(fd);
                break;
            }
        }
        co_return std::move(len);
    }

    /// @brief 0 is success -1 is failed 1 is suspend
    /// @param domain 
    /// @param fd 
    /// @param ip 
    /// @param port 
    /// @return 
    static Coroutine<int> co_connect(int domain,int fd,const char* ip , int port)
    {
        sockaddr_in saddr;
        saddr.sin_family = domain;
        saddr.sin_port = htons(port);
        saddr.sin_addr.s_addr = inet_addr(ip);
        int ret = SUSPEND;
        while (1)
        {
            co_yield ret;
            ret = connect(fd,(sockaddr*)&saddr,sizeof(sockaddr));
            if(ret == 0 || (ret == -1 && errno == EISCONN) ) break;
            else if(ret == -1 && errno == EINPROGRESS) {}
            else co_return std::move(-1);
        }
        co_return std::move(0);
    }

};


#endif // !__GALAY_CO_FUNCTION_H__
