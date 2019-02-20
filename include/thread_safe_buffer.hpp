#ifndef THREAD_SAFE_BUFFER_HPP
#define THREAD_SAFE_BUFFER_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class Thread_Safe_Buffer
{
public:
	using u_lock = std::unique_lock<std::mutex>;
private:
	std::queue<T> m_queue;
	mutable std::mutex m_mutex;
	std::condition_variable m_cond_var;

public:
	Thread_Safe_Buffer():m_queue()
	{}

	~Thread_Safe_Buffer()
	{
		while(!m_queue.empty())
			m_queue.pop();

	}

	void size() const
	{
		u_lock lock(m_mutex);
		return m_queue.size();
	}
	void push_item(const T&item)
	{
		u_lock lock(m_mutex);
		m_queue.push(item);
		m_cond_var.notify_all();
	}
	void push_item(T &&item)
	{
		u_lock lock(m_mutex);
		m_queue.push(std::move(item));
		m_cond_var.notify_all();
	}
	bool try_pop(T &item)
	{
		u_lock lock(m_mutex);
		if (m_queue.empty())
			return false;
		else
		{
			item = std::move(m_queue.front());
			m_queue.pop();
			return true;
		}
	}

	void wait_and_pop(T &item)
	{
		u_lock lock(m_mutex);
		if(m_queue.empty())
		{
			m_cond_var.wait(lock);
		}
		item = std::move(m_queue.front());
		m_queue.pop();
	}

	void reset() 
	{
		u_lock lock(m_mutex);
		while (!m_queue.empty())
			m_queue.pop();
		m_cond_var.notify_all();

	}
};


#endif