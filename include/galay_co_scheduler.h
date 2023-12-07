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

#define MAX_EVENT_SIZE					1024
#define COROUTINE_INITIAL_SIZE			10
#define COROUTINE_EXTEND_SIZE			11


template<typename RESULT>
class Co_Scheduler
{
public:
	using Ptr = std::shared_ptr<Co_Scheduler>;
	virtual void add_coroutine(uint32_t fd, MainCoroutine<RESULT>* co) = 0;
	virtual int get_coroutine(uint32_t fd,MainCoroutine<RESULT>* co) = 0;
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
		m_coroutines.assign(COROUTINE_INITIAL_SIZE,nullptr);
	}

	Co_Net_Scheduler(Co_Net_Scheduler<RESULT>&& other) = delete;
	Co_Net_Scheduler(Co_Net_Scheduler<RESULT>& other) = delete;
	

public:
	using Ptr = std::shared_ptr<Co_Net_Scheduler<RESULT>>;

	static Co_Net_Scheduler<RESULT>* GetInstance(std::string name)
	{
		return new Co_Net_Scheduler<RESULT>(name);
	}

	void add_coroutine(uint32_t fd , MainCoroutine<RESULT>* co) override
	{
		if(fd >= m_coroutines.size()){
			m_coroutines.resize( fd + COROUTINE_EXTEND_SIZE);
		}
		m_coroutines[fd] = co;
	}

	void del_coroutine(uint32_t fd)
	{
		if(m_coroutines[fd] != nullptr){
			delete m_coroutines[fd];
		}
		m_coroutines[fd] = nullptr;
	}

	int get_coroutine(uint32_t fd,MainCoroutine<RESULT>* co) override
	{
		if(fd >= m_coroutines.size()) return -1;
		else {
			if(m_coroutines[fd] == nullptr) return -1;
			else co = m_coroutines[fd];
		}
		return 0;
	}


	void run(int timeout = -1) override
	{
		while (!m_stop.load())
		{
			int nready = epoll_wait(this->epfd,this->events,MAX_EVENT_SIZE,timeout);
			if(nready == 0) continue;
			for(int i = 0 ;i < nready ;i++)
			{
				cur_event = events + i;
				m_coroutines[events[i].data.fd]->resume();
			}
		}
	}

	bool is_stop(){
		return this->m_stop.load();
	}

	void stop() override
	{
		m_stop.store(true,std::memory_order_seq_cst);
		for (auto &ptr : m_coroutines)
		{
			if(ptr) {
				ptr->promise().set_status(TERM);
				if(!ptr->done()) ptr->resume();
			}
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
		for(int i = 3 ; i < m_coroutines.size() ; i++)
		{
			if(m_coroutines[i]){
				delete m_coroutines[i];
				close(i);
				del_epoll(i,EPOLLIN | EPOLLOUT);
			}
		}
	}

	int add_epoll(int fd , uint32_t event)
	{
		epoll_event ev;
		ev.data.fd = fd;
		ev.events = event;
		std::cout<< fd <<"add epoll success\n";
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
	std::vector<MainCoroutine<RESULT>*> m_coroutines;
};


// extern Co_Net_Scheduler<int>* scheduler;

#endif // !__GALAY_CO_SCHEDULER_H__
