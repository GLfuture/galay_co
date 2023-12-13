#ifndef __GALAY_CO_H__
#define __GALAY_CO_H__

#include <coroutine>
#include <functional>
#include <iostream>
#include <memory>

static uint64_t global_co_id = 0;

template<typename RESULT>
class Scheduler;

template<typename RESULT>
class Coroutine;

//承诺类
template<typename RESULT>
class Promise {
public:
	static auto get_return_object_on_alloaction_failure() {
		return nullptr;
	}

	auto get_return_object() { return std::coroutine_handle<Promise>::from_promise(*this); }

	std::suspend_never initial_suspend() noexcept { 
		return {}; 
	}

    auto yield_value(const RESULT& value){
		m_result = value;
        return std::suspend_always{};
    }

	auto yield_value(RESULT&& value){
		m_result = value;
        return std::suspend_always{};
    }

	std::suspend_always final_suspend() noexcept { 
		return {}; 
	}

	void unhandled_exception() noexcept {
		m_exception = std::current_exception();
	}

	void return_value(RESULT&& val) noexcept { 
		this->m_result = val;
	}
	
	//获取结果
	RESULT& result() {
		rethrow_if_exception();
		return this->m_result;
	}

private:
	void rethrow_if_exception()
	{
		if (m_exception)
		{
			std::rethrow_exception(m_exception);
		}
	}
private:
	std::exception_ptr m_exception = nullptr;
	RESULT m_result;
};

template<>
class Promise<void>
{
public:
	static auto get_return_object_on_alloaction_failure() {
		return nullptr;
	}

	auto get_return_object() { return std::coroutine_handle<Promise>::from_promise(*this); }

	std::suspend_never initial_suspend() noexcept { 
		return {}; 
	}

    template<typename T>
    auto yield_value(const T& value){
        return std::suspend_always{};
    }


	std::suspend_always final_suspend() noexcept { 
		return {}; 
	}

	void unhandled_exception() noexcept {
		m_exception = std::current_exception();
	}

	void return_void() noexcept { }

	//获取结果
	void result() {
		rethrow_if_exception();
	}

private:
	void rethrow_if_exception()
	{
		if (m_exception)
		{
			std::rethrow_exception(m_exception);
		}
	}
private:
	std::exception_ptr m_exception = nullptr;
};

template<typename RESULT>
class Coroutine
{
public:
	using promise_type = Promise<RESULT>;

	Coroutine<RESULT> &operator=(Coroutine<RESULT>&& other)
	{
		this->co_handle = other.co_handle;
		this->co_id = other.co_id;
		return *this;
	}

	Coroutine(){}

	Coroutine(std::coroutine_handle<promise_type> co_handle) noexcept
	{
		this->co_handle = co_handle;
        this->co_id = global_co_id ++;
	}

	Coroutine(Coroutine<RESULT>&& other) noexcept
	{
		this->co_handle = other.co_handle;
		this->co_id = other.co_id;
	}

	static Coroutine<RESULT> creat(std::function<Coroutine<RESULT>()>&& func)
	{
		return Coroutine<RESULT>(func());
	}

	//返回承诺体,包含返回值
	promise_type& promise()
	{
		return co_handle.promise();
	}

	void resume() noexcept
	{
		co_handle.resume();
	}

	bool done() noexcept
	{
		return co_handle.done();
	}

    uint64_t get_co_id()
    {
        return this->co_id;
    }

	~Coroutine()
	{
		if (co_handle) {
			co_handle.destroy();
		}
	}

	Coroutine(const Coroutine& other) = delete;

	Coroutine& operator=(Coroutine& other) = delete;
	
protected:
	//协程id
	uint64_t co_id = 0;
	//协程句柄
	std::coroutine_handle<promise_type> co_handle = nullptr;
};

//主协程
template<typename RESULT>
class MainCoroutine:public Coroutine<RESULT>
{
public:
	using promise_type = Promise<RESULT>;

	MainCoroutine<RESULT> &operator=(MainCoroutine<RESULT>&& other)
	{
		this->co_handle = other.co_handle;
		this->co_id = other.co_id;
		return *this;
	}

	MainCoroutine(){}

	MainCoroutine(std::coroutine_handle<promise_type> co_handle) noexcept
	{
		this->co_handle = co_handle;
        this->co_id = global_co_id ++;
	}

	MainCoroutine(MainCoroutine<RESULT>&& other) noexcept
	{
		this->co_handle = other.co_handle;
		this->co_id = other.co_id;
	}

	static MainCoroutine<RESULT> creat(std::function<MainCoroutine<RESULT>()>&& func)
	{
		return MainCoroutine<RESULT>(func());
	}
};


//等待体类
class Awaiter
{
public:
	virtual bool await_ready() { return false; }
	virtual void await_resume() { }
	virtual void await_suspend(std::coroutine_handle<> co_handle) {}
};

#endif


