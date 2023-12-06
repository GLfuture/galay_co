#ifndef __GALAY_CO_SCHEDULER_H__
#define __GALAY_CO_SCHEDULER_H__
#include "galay_co.h"
#include "galay_co_result.h"
#include <map>
#include <list>
#include <string>
#include <thread>
#include <vector>
#include <assert.h>
#include <atomic>
#include <sys/epoll.h>

#define MAX_EVENT_SIZE		1024


template<typename RESULT>
class Co_Scheduler
{
public:
	using Ptr = std::shared_ptr<Co_Scheduler>;
	virtual void add_coroutine(uint32_t fd, Coroutine<RESULT>* co) = 0;
	virtual int get_coroutine(uint32_t fd,Coroutine<RESULT>* co) = 0;
	virtual void stop() = 0;
	virtual void run(int timeout = -1) = 0;
	virtual ~Co_Scheduler(){}
};

template<typename RESULT>
class Co_Net_Scheduler : public Co_Scheduler<RESULT>
{
private:
	Co_Net_Scheduler(const std::string& name)
	{
		this->m_name = name;
		this->epfd = epoll_create(1024);
	}

	Co_Net_Scheduler(Co_Net_Scheduler<RESULT>&& other) = delete;
	Co_Net_Scheduler(Co_Net_Scheduler<RESULT>& other) = delete;
	

public:
	using Ptr = std::shared_ptr<Co_Net_Scheduler<RESULT>>;

	static Co_Net_Scheduler<RESULT>* GetInstance(std::string name)
	{
		return new Co_Net_Scheduler<RESULT>(name);
	}

	void add_coroutine(uint32_t fd , Coroutine<RESULT>* co) override
	{
		m_coroutines[fd] = co;
	}

	void del_coroutine(uint32_t fd)
	{
		typename std::map<uint32_t,Coroutine<RESULT>*>::iterator it = m_coroutines.find(fd);
		if(it == m_coroutines.end()) return;
		delete it->second;
		it->second = nullptr;
		m_coroutines.erase(it);
	}

	int get_coroutine(uint32_t fd,Coroutine<RESULT>* co) override
	{
		auto it = m_coroutines.find(fd);
		if (it == m_coroutines.end()) return -1;
		co = it->second;
		return 0;
	}


	void run(int timeout = -1) override
	{
		while (!m_stop.load())
		{
			int nready = epoll_wait(this->epfd,this->events,MAX_EVENT_SIZE,timeout);
			while(nready -- > 0)
			{
				cur_event = events + nready;
				typename std::map<uint32_t,Coroutine<RESULT>*>::iterator it = m_coroutines.find(events[nready].data.fd);
				if(it != m_coroutines.end()){
					it->second->resume();
				}
			}
		}
	}

	bool is_stop(){
		return this->m_stop.load();
	}

	void stop() override
	{
		m_stop.store(true,std::memory_order_seq_cst);
		for (auto &[ _ , value] : m_coroutines)
		{
			value->promise().set_status(TERM);
			if(!value->done()) value->resume();
		}
	}

	int get_epoll_fd()
	{
		return this->epfd;
	}

	epoll_event* get_cur_event()
	{
		return this->cur_event;
	}

	~Co_Net_Scheduler()
	{
		assert(m_stop.load());
		for(typename std::map<uint32_t,Coroutine<RESULT>*>::iterator it = m_coroutines.begin();
			it != m_coroutines.end();it++)
		{
			delete it->second;
			it->second = nullptr;
			epoll_event ev;
			ev.data.fd = it->first;
			ev.events = EPOLLIN;
			epoll_ctl(this->epfd,EPOLL_CTL_DEL,it->first,&ev);
			it = m_coroutines.erase(it);
			if(it == m_coroutines.end()){
				break;
			}
			it -- ;
		}
	}

	int add_epoll(int fd , uint32_t event)
	{
		epoll_event ev;
		ev.data.fd = fd;
		ev.events = event;
		return epoll_ctl(this->epfd,EPOLL_CTL_ADD,fd,&ev);
	}

	int del_epoll(int fd , uint32_t event)
	{
		epoll_event ev;
		ev.data.fd = fd;
		ev.events = event;
		return epoll_ctl(this->epfd,EPOLL_CTL_DEL,fd,&ev);
	}

	int mod_epoll(int fd , uint32_t event)
	{
		epoll_event ev;
		ev.data.fd = fd;
		ev.events = event;
		return epoll_ctl(this->epfd,EPOLL_CTL_MOD,fd,&ev);
	}

protected:
	int epfd;
	epoll_event* cur_event = nullptr;
	epoll_event events[MAX_EVENT_SIZE];
	std::string m_name;
	std::atomic_bool m_stop = false;
	std::map<uint32_t,Coroutine<RESULT>*> m_coroutines;
};


extern Co_Net_Scheduler<int>* scheduler;

#endif // !__GALAY_CO_SCHEDULER_H__
