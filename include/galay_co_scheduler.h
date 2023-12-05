#ifndef __GALAY_CO_SCHEDULER_H__
#define __GALAY_CO_SCHEDULER_H__
#include "galay_co.h"
#include <map>
#include <list>
#include <string>
#include <thread>
#include <vector>
#include <assert.h>
#include <atomic>


template<typename RESULT>
class Co_Scheduler
{
public:
	using Ptr = std::shared_ptr<Co_Scheduler>;
	virtual void add_coroutine(uint32_t fd, std::shared_ptr<Coroutine<RESULT>>&& co) = 0;
	virtual std::shared_ptr<Coroutine<RESULT>> get_coroutine(uint32_t fd) = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void run() = 0;
	virtual ~Co_Scheduler(){}
};

template<typename RESULT>
class Co_Net_Scheduler : public Co_Scheduler<RESULT>
{
private:
	Co_Net_Scheduler(const std::string& name)
	{
		this->m_name = name;
	}

	Co_Net_Scheduler(Co_Net_Scheduler<RESULT>&& other) = delete;
	Co_Net_Scheduler(Co_Net_Scheduler<RESULT>& other) = delete;
	

public:
	using Ptr = std::shared_ptr<Co_Net_Scheduler<RESULT>>;

	static Co_Net_Scheduler<RESULT>* GetInstance(std::string name)
	{
		return new Co_Net_Scheduler<RESULT>(name);
	}

	void add_coroutine(uint32_t fd , std::shared_ptr<Coroutine<RESULT>>&& co) override
	{
		m_coroutines[fd] = co;
	}

	std::shared_ptr<Coroutine<RESULT>> get_coroutine(uint32_t fd) override
	{
		auto it = m_coroutines.find(fd);
		if (it == m_coroutines.end()) return nullptr;
		return it->second;
	}


	void run() override
	{
		while (!m_stop.load())
		{
			
		}
		
	}

	void stop() override
	{
		m_stop.store(true,std::memory_order_seq_cst);
		for (auto &[ _ , value] : m_coroutines)
		{
			value->resume();
		}
	}

	~Co_Net_Scheduler()
	{
		assert(m_stop.load());
		m_threads.clear();
		m_coroutines.clear();
	}
protected:
	std::string m_name;
	std::atomic_bool m_stop = false;
	std::vector<std::thread> m_threads;
	std::map<uint32_t,std::list<Coroutine<RESULT>>> m_coroutines;
};


#endif // !__GALAY_CO_SCHEDULER_H__
