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
            printf("Failed to create a new socket\n");
            return -1;
        }
        int ret = fcntl(fd, F_SETFL, O_NONBLOCK);
        if (ret == -1)
        {
            close(ret);
            return -1;
        }
        int option = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&option, sizeof(option));
        return fd;
    }

    static Coroutine<Net_Result::Ptr> co_accept(int fd , sockaddr* addr , socklen_t* len)
    {
        int ret = accept(fd,addr,len);
        Net_Result::Ptr res = std::make_shared<Net_Result>(ret);
        co_return std::move(res);
    }

    static Coroutine<Net_Result::Ptr> co_recv(int fd, void *buf, size_t n, int flags)
    {
        int ret = recv(fd, buf, n, flags);
        Net_Result::Ptr res = std::make_shared<Net_Result>(ret);
        co_return std::move(res);
    }

    static Coroutine<Net_Result::Ptr> co_send(int fd, void *buf, size_t n, int flags)
    {
        int ret = send(fd, buf, n, flags);
        Net_Result::Ptr res = std::make_shared<Net_Result>(ret);
        co_return std::move(res);
    }

private:

    
};



#endif // !__GALAY_CO_FUNCTION_H__
