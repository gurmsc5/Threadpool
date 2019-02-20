#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include "thread_safe_buffer.hpp"
#include <functional>
#include <future>
#include <memory>


namespace gsc_concurrency
{
	class worker_thread
	{
	public:
		using t_buf = Thread_Safe_Buffer<std::function<void()>>;
		using s_buf_ptr = std::shared_ptr<t_buf>;
	private:
		std::vector<std::thread> m_workers;
		size_t m_threads;
		s_buf_ptr m_tasks_buf;
		std::atomic<bool> shutdown;
	public:

		worker_thread(const size_t threads, s_buf_ptr &buf) :
			m_threads(threads)
			, m_tasks_buf(buf)
			, shutdown(false)
		{}
		void start_threads()
		{
			for (size_t i{ 0 }; i < m_threads; ++i)
			{
				m_workers.emplace_back(
					[this]()
				{
					while (!shutdown)
					{
						std::function<void()> task;
						if(m_tasks_buf->try_pop(task))
							task();
					}
				}
				);
			}
		}
		void shutdown_threads()
		{
			shutdown = true;
			m_tasks_buf->reset();
			for (auto &worker : m_workers)
			{
				if (worker.joinable())
					worker.join();
			}
		}
	};

	class Threadpool
	{
		
	public:
		/*alias for better readability*/
		using t_buf = Thread_Safe_Buffer<std::function<void()>>;
		using s_buf_ptr = std::shared_ptr<t_buf>;

		Threadpool(const size_t pool_size) :
			m_pool_size(pool_size)
			,m_tasks(std::make_shared<t_buf>())
			,m_worker(pool_size,m_tasks)
		{}
		~Threadpool()
		{
			m_worker.shutdown_threads();
		}
		void start_pool()
		{
			m_worker.start_threads();
		}

		template<typename Func, typename...ARGS>
		auto add_task(Func &&func, ARGS ...args)->std::future<decltype(func(args...))>
		{
			auto task = std::make_shared<std::packaged_task<decltype(func(args...))()>>(
				std::bind(std::forward<Func>(func), std::forward<ARGS>(args)...)
				);

			m_tasks->push_item([task]() {
				(*task)();
			});

			return task->get_future();
		}



	private:
		size_t m_pool_size;
		s_buf_ptr m_tasks;
		worker_thread m_worker;
	};



}



#endif